#include <Python.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <datetime.h>
#include <time.h>

// DFCom API
#include <DFComAPI.h>
#include <AEErrorDef.h>

#include <boost/python.hpp>
using namespace boost::python;

#define DFC_COMNUM 1
#define DFC_BUSNUM 254
#define DFC_TCP 3


namespace
{
	void _dfc_error(int error)
	{
		char msg[254];
		memset(msg, 0, 254);

		DFCGetErrorText(DFC_COMNUM, error, 0, msg, 254);
		throw std::runtime_error(msg);
	}

	PyObject *_decode_buffer(unsigned char* buf, int len, PyObject *desc)
	{
		if(!desc || !PyList_Check(desc))
		{
			throw std::runtime_error("desc-argument is no list");
		}

		PyObject *item = PyDict_New();

		Py_ssize_t nfields = PyList_Size(desc);
		for(Py_ssize_t nfield = 0; nfield < nfields; nfield++)
		{
			PyObject *field = PyList_GetItem(desc, nfield);
			PyObject *v;

			if(!field || !PyDict_Check(field))
			{
				PyObject_Free(item);
				throw std::runtime_error("desc-list contains an item that is no dict");
			}


			v = PyDict_GetItemString(field, "type");
			if(!v || !PyLong_Check(v))
			{
				PyObject_Free(item);
				throw std::runtime_error("desc-list contains a dict without a 'type' member or with a 'type' member which is no long");
			}
			long type = PyLong_AsLong(v);


			v = PyDict_GetItemString(field, "size");
			if(!v || !PyLong_Check(v))
			{
				PyObject_Free(item);
				throw std::runtime_error("desc-list contains a dict without a 'size' member or with a 'size' member which is no long");
			}
			long size = PyLong_AsLong(v);

			if(len < size)
			{
				throw std::runtime_error("field definition is larget then buffer");
			}

			switch(type)
			{
				case 3:  // string
					v = PyUnicode_DecodeLatin1((char*)buf, size-1, NULL);
					break;

				case 2:  // date
					v = PyDateTime_FromDateAndTime(
						/* year */  buf[0] * 100 + buf[1],
						/* month */ buf[2],
						/* day */   buf[3],
						/* hour */  buf[4],
						/* minute */buf[5],
						/* second */buf[6],
						/* usec */  0);
					break;

				default:
					Py_INCREF(Py_None);
					v = Py_None;
					break;
			}

			len -= size;
			buf += size;

			PyObject *k = PyDict_GetItemString(field, "fieldname");
			if(!k || !PyUnicode_Check(k))
			{
				PyObject_Free(item);
				throw std::runtime_error("desc-list contains a dict without a 'name' member or with a 'name' member which is no str");
			}

			PyDict_SetItem(item, k, v);
		}

		return item;
	}



	void connect(std::string host, int port, unsigned int read_timeout_ms)
	{
		char *chost = strdup(host.c_str());
		if(! DFCComOpenIV(DFC_COMNUM, DFC_BUSNUM, DFC_TCP, chost, port, read_timeout_ms)) {
			throw std::runtime_error("could not connect to host");
		}

		free(chost);
	}

	int get_serial()
	{
		int error = 0, serial = 0;
		if(! DFCGetSeriennummer(DFC_COMNUM, DFC_BUSNUM, &error, &serial))
		{
			_dfc_error(error);
			return -1;
		}

		return serial;
	}

	PyObject *get_datetime()
	{
		unsigned char buf[7];

		if (! DFCComGetTime(DFC_COMNUM, DFC_BUSNUM, buf)) {
			throw std::runtime_error("error reading date/time");
			return NULL;
		}

		return PyDateTime_FromDateAndTime(
			/* year */  buf[0] * 100 + buf[1],
			/* month */ buf[2],
			/* day */   buf[3],
			/* hour */  buf[4],
			/* minute */buf[5],
			/* second */buf[6],
			/* usec */  0);
	}

	void set_datetime(PyObject *datetime)
	{
		if(!datetime || !PyDateTime_Check(datetime))
		{
			throw std::runtime_error("argument is non instance of datetime.datetime");
		}

		unsigned char buf[7];

		int year = PyDateTime_GET_YEAR(datetime);
		buf[0] = year / 100;
		buf[1] = year % 100;
		buf[2] = PyDateTime_GET_MONTH(datetime);
		buf[3] = PyDateTime_GET_DAY(datetime);
		buf[4] = PyDateTime_DATE_GET_HOUR(datetime);
		buf[5] = PyDateTime_DATE_GET_MINUTE(datetime);
		buf[6] = PyDateTime_DATE_GET_SECOND(datetime);

		if (! DFCComSetTime(DFC_COMNUM, DFC_BUSNUM, buf)) {
			throw std::runtime_error("error setting date/time");
		}
	}

	PyObject *read_list_descriptions()
	{
		int error, zero;

		if(!DFCLoadListenbeschreibung(DFC_COMNUM, DFC_BUSNUM, &error))
		{
			_dfc_error(error);
			return NULL;
		}

		int ndescs = DFCListBCnt(DFC_COMNUM);
		PyObject *descs = PyList_New(ndescs);

		for(int ndesc = 0; ndesc < ndescs; ndesc++)
		{
			int nfields;
			unsigned char listname[17];

			DFCListBDatensatz(DFC_COMNUM, ndesc, listname, &nfields, &zero);

			PyObject *fields = PyList_New(nfields);
			PyList_SetItem(descs, ndesc, fields);

			for(int nfield = 0; nfield < nfields; nfield++)
			{
				int type, size;
				unsigned char fieldname[17];

				DFCListBFeld(DFC_COMNUM, ndesc, nfield, fieldname, &type, &size);

				PyObject *field = PyDict_New();
				PyList_SetItem(fields, nfield, field);

				PyDict_SetItemString(field, "fieldname",
					PyUnicode_DecodeLatin1((const char*)fieldname, strlen((const char*)fieldname), NULL));

				PyDict_SetItemString(field, "type",
					PyLong_FromLong(type));

				PyDict_SetItemString(field, "size",
					PyLong_FromLong(size));
			}
		}

		return descs;
	}

	void push_list(int list_id, PyObject *desc, PyObject *data)
	{
		if(!desc || !PyList_Check(desc))
		{
			throw std::runtime_error("desc-argument is no list");
		}

		if(!data || !PyList_Check(data))
		{
			throw std::runtime_error("data-argument is no list");
		}

		Py_ssize_t nfields = PyList_Size(desc);
		Py_ssize_t nlines = PyList_Size(data);

		PyObject **field_names = (PyObject **)calloc(nfields, sizeof(PyObject*));
		Py_ssize_t* field_length_ints = (Py_ssize_t*)calloc(nfields, sizeof(Py_ssize_t));
		Py_ssize_t  line_length = 0;

		PyObject *field;

		for(Py_ssize_t nfield = 0; nfield < nfields; nfield++)
		{
			PyObject *v;

			field = PyList_GetItem(desc, nfield);
			if(!field || !PyDict_Check(field))
			{
				free(field_names);
				free(field_length_ints);
				throw std::runtime_error("desc-list contains an item that is no dict");
			}

			v = PyDict_GetItemString(field, "type");
			if(!v || !PyLong_Check(v))
			{
				free(field_names);
				free(field_length_ints);
				throw std::runtime_error("desc-list contains a dict without a 'type' member or with a 'type' member which is no long");
			}
			if(4 != PyLong_AsLong(v))
			{
				free(field_names);
				free(field_length_ints);
				throw std::runtime_error("desc-list contains a dict that has a type-member that is not == 4 (string). pusing non-string-fields is not implemented yet.");
			}

			v = PyDict_GetItemString(field, "fieldname");
			if(!v || !PyUnicode_Check(v))
			{
				free(field_names);
				free(field_length_ints);
				throw std::runtime_error("desc-list contains a dict without a 'fielname' member or with a 'fielname' member which is no str");
			}
			field_names[nfield] = v;

			v = PyDict_GetItemString(field, "size");
			if(!v || !PyLong_Check(v))
			{
				free(field_names);
				free(field_length_ints);
				throw std::runtime_error("desc-list contains a dict without a 'size' member or with a 'size' member which is no long");
			}

			Py_ssize_t field_length = PyLong_AsSize_t(v);

			field_length_ints[nfield] = field_length - 1;
			line_length += field_length;
		}

		Py_ssize_t bufsz = line_length * nlines;
		unsigned char* buf = (unsigned char*)calloc(bufsz, 1);
		if(! buf)
		{
			free(buf);
			free(field_names);
			free(field_length_ints);
			throw std::runtime_error("can't allocate buffer");
		}

		Py_ssize_t offset = 0;
		for(Py_ssize_t nline = 0; nline < nlines; nline++)
		{
			PyObject *line = PyList_GetItem(data, nline);
			if(!line || !PyDict_Check(line))
			{
				free(buf);
				free(field_names);
				free(field_length_ints);
				throw std::runtime_error("data-argument contains an item that is no dict");
			}

			for(Py_ssize_t nfield = 0; nfield < nfields; nfield++)
			{
				std::string fname;
				fname.assign(PyUnicode_AsUTF8(field_names[nfield]));

				field = PyDict_GetItem(line, field_names[nfield]);
				if(!field)
				{
					free(buf);
					free(field_names);
					free(field_length_ints);
					throw std::runtime_error(
						std::string("a line in the data-argument misses the field '") + fname + std::string("'"));
				}

				if(!PyUnicode_Check(field))
				{
					free(buf);
					free(field_names);
					free(field_length_ints);
					throw std::runtime_error(
						std::string("a line in the data-argument has a field '") + fname + std::string("' which does not contain a string"));
				}

				PyObject *value_latin1_bytes = PyUnicode_AsLatin1String(field);
				char *value_latin1 = PyBytes_AsString(value_latin1_bytes);
				//char *value_utf8 = PyUnicode_AsUTF8(field);

				strcpy((char*) &buf[offset], value_latin1);
				offset += field_length_ints[nfield] + 1;
			}
		}

		DFCClrListenBuffer(DFC_COMNUM);
		if(! DFCMakeListe(DFC_COMNUM, list_id, nlines, bufsz, buf, 0))
		{
			free(buf);
			free(field_length_ints);
			free(field_names);

			throw std::runtime_error("error preparing list-buffer");
		}

		int error;
		if (!DFCLoadListen(DFC_COMNUM, DFC_BUSNUM, &error)) {
			free(buf);
			free(field_length_ints);
			free(field_names);

			_dfc_error(error);
		}


		free(buf);
		free(field_length_ints);
		free(field_names);
	}

	PyObject *read_data_descriptions()
	{
		int error;

		if(!DFCLoadDatensatzbeschreibung(DFC_COMNUM, DFC_BUSNUM, &error))
		{
			_dfc_error(error);
			return NULL;
		}

		int ndescs = DFCDatBCnt(DFC_COMNUM);
		PyObject *descs = PyList_New(ndescs);

		for(int ndesc = 0; ndesc < ndescs; ndesc++)
		{
			int nfields;
			unsigned char listname[17];

			DFCDatBDatensatz(DFC_COMNUM, ndesc, listname, &nfields);

			PyObject *fields = PyList_New(nfields);
			PyList_SetItem(descs, ndesc, fields);

			for(int nfield = 0; nfield < nfields; nfield++)
			{
				int type, size;
				unsigned char fieldname[17];

				DFCDatBFeld(DFC_COMNUM, ndesc, nfield, fieldname, &type, &size);

				PyObject *field = PyDict_New();
				PyList_SetItem(fields, nfield, field);

				PyDict_SetItemString(field, "fieldname",
					PyUnicode_DecodeLatin1((const char*)fieldname, strlen((const char*)fieldname), NULL));

				PyDict_SetItemString(field, "type",
					PyLong_FromLong(type));

				PyDict_SetItemString(field, "size",
					PyLong_FromLong(size));
			}
		}

		return descs;
	}

	PyObject *read_data(PyObject *desc)
	{
		int error;

		if(!desc || !PyList_Check(desc))
		{
			throw std::runtime_error("desc-argument is no list");
		}

		PyObject *items = PyList_New(0);

		switch (DFCComCollectData(DFC_COMNUM, DFC_BUSNUM, &error)) {
			// error occured
			case -1:
				PyObject_Free(items);
				_dfc_error(error);
				return NULL;

			// nothing there
			case 0:
				return items;
		}

		int len, ret;
		unsigned char buf[0xFF];
		do {
			switch (ret = DFCComGetDatensatz(DFC_COMNUM, buf, &len, &error))
			{
				case -1:
					PyObject_Free(items);
					_dfc_error(error);
					return NULL;

				case 0:
					break;

				case 1: // Datensatz in Folge.
				case 2: // Letzter Datensatz.
					int desc_id = buf[0];
					PyObject *fielddesc = PyList_GetItem(desc, desc_id);

					PyObject *item = _decode_buffer(&buf[1], len-1, fielddesc);
					PyList_Append(items, item);
					break;
			}
		} while (ret == 1);

		return items;
	}

	void disconnect()
	{
		DFCComClose(DFC_COMNUM);
	}
}

BOOST_PYTHON_MODULE(pydfcom)
{
	PyDateTimeAPI = (PyDateTime_CAPI *)PyCapsule_Import(PyDateTime_CAPSULE_NAME, 0);

	boost::python::scope().attr("TYPE_DATE") = 3;
	boost::python::scope().attr("TYPE_STRING") = 2;

	def("connect", connect);

	def("get_serial", get_serial);

	def("get_datetime", get_datetime);
	def("set_datetime", set_datetime);

	def("read_list_descriptions", read_list_descriptions);
	def("push_list", push_list);

	def("read_data_descriptions", read_data_descriptions);
	def("read_data", read_data);

	def("disconnect", disconnect);
}

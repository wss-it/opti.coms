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
		if(! PyList_Check(desc))
		{
			throw std::runtime_error("desc-argument is not a list");
		}

		PyObject *item = PyDict_New();

		Py_ssize_t nfields = PyList_Size(desc);
		for(Py_ssize_t nfield = 0; nfield < nfields; nfield++)
		{
			PyObject *field = PyList_GetItem(desc, nfield);
			PyObject *v;

			if(! PyDict_Check(field))
			{
				PyObject_Free(item);
				throw std::runtime_error("desc-list contains an item that is not a dict");
			}


			v = PyDict_GetItemString(field, "type");
			if(! PyLong_Check(v))
			{
				PyObject_Free(item);
				throw std::runtime_error("desc-list contains an dict with a member 'type' which is no int");
			}
			long type = PyLong_AsLong(v);


			v = PyDict_GetItemString(field, "size");
			if(! PyLong_Check(v))
			{
				PyObject_Free(item);
				throw std::runtime_error("desc-list contains an dict with a member 'size' which is no int");
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
			if(! PyUnicode_Check(k))
			{
				PyObject_Free(item);
				throw std::runtime_error("desc-list contains an dict with a member 'name' which is no str");
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
		if(! PyDateTime_Check(datetime))
		{
			throw std::runtime_error("argument is not an instance of datetime.datetime");
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

	void push_list(int list_id, PyObject *field_lengths, PyObject *data)
	{
		if(! PyList_Check(field_lengths))
		{
			throw std::runtime_error("field_lengths-argument is not a list");
		}

		if(! PyList_Check(data))
		{
			throw std::runtime_error("data-argument is not a list");
		}

		Py_ssize_t nfields = PyList_Size(field_lengths);
		Py_ssize_t nlines = PyList_Size(data);

		Py_ssize_t* field_length_ints = (Py_ssize_t*)calloc(nfields, sizeof(Py_ssize_t));
		Py_ssize_t  line_length = 0;

		PyObject *field;

		for(Py_ssize_t nfield = 0; nfield < nfields; nfield++)
		{
			field = PyList_GetItem(field_lengths, nfield);
			if(! PyNumber_Check(field))
			{
				throw std::runtime_error("field_lengths-list contains an item that is not a number");
			}

			Py_ssize_t field_length = PyNumber_AsSsize_t(field, NULL);

			field_length_ints[nfield] = field_length;
			line_length += field_length + 1;
		}

		Py_ssize_t bufsz = line_length * nlines;
		unsigned char* buf = (unsigned char*)calloc(bufsz, 1);
		if(! buf)
		{
			throw std::runtime_error("can't allocate buffer");
		}

		Py_ssize_t offset = 0;
		for(Py_ssize_t nline = 0; nline < nlines; nline++)
		{
			PyObject *line = PyList_GetItem(data, nline);
			if(! PyList_Check(line))
			{
				throw std::runtime_error("data-argument contains an item that is not a list");
			}

			for(Py_ssize_t nfield = 0; nfield < nfields; nfield++)
			{
				field = PyList_GetItem(line, nfield);
				if(! PyUnicode_Check(field))
				{
					throw std::runtime_error("a line in the data-argument contains an item that is not a string");
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

			throw std::runtime_error("error preparing list-buffer");
		}

		int error;
		if (!DFCLoadListen(DFC_COMNUM, DFC_BUSNUM, &error)) {
			free(buf);
			free(field_length_ints);

			_dfc_error(error);
		}


		free(buf);
		free(field_length_ints);
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

		if(! PyList_Check(desc))
		{
			throw std::runtime_error("desc-argument is not a list");
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

	def("push_list", push_list);

	def("read_data_descriptions", read_data_descriptions);
	def("read_data", read_data);

	def("disconnect", disconnect);
}

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

		if (!DFCComSetTime(DFC_COMNUM, DFC_BUSNUM, buf)) {
			throw std::runtime_error("error setting date/time");
		}
	}

	void disconnect()
	{
		DFCComClose(DFC_COMNUM);
	}
}

BOOST_PYTHON_MODULE(pydfcom)
{
	PyDateTimeAPI = (PyDateTime_CAPI *)PyCapsule_Import(PyDateTime_CAPSULE_NAME, 0);

	def("connect", connect);
	def("get_serial", get_serial);
	def("get_datetime", get_datetime);
	def("set_datetime", set_datetime);
	def("disconnect", disconnect);
}

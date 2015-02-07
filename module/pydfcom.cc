#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
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
	void raise_dfc_exception(int error)
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
			raise_dfc_exception(error);
			return -1;
		}

		return serial;
	}

	void disconnect()
	{
		DFCComClose(DFC_COMNUM);
	}
}

BOOST_PYTHON_MODULE(pydfcom)
{
	def("connect", connect);
	def("get_serial", get_serial);
	def("disconnect", disconnect);
}

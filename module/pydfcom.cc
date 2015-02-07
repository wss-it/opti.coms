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


namespace {
	int get_serial(std::string host, int port) {
		char *chost = strdup(host.c_str());
		if(! DFCComOpenIV(DFC_COMNUM, DFC_BUSNUM, DFC_TCP, chost, port, 3000)) {
			free(chost);
			return -1;
		}
		free(chost);

		int error = 0, serial = 0;
		if(! DFCGetSeriennummer(DFC_COMNUM, DFC_BUSNUM, &error, &serial)) {
			// DFCGetErrorText(DFC_COMNUM, error, 0, g_CommonInfo.szErrorMsg, 254);

			return -2;
		}

		DFCComClose(DFC_COMNUM);
		return serial;
	}
}

BOOST_PYTHON_MODULE(pydfcom)
{
	def("get_serial", get_serial);
}

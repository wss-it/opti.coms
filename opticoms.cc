#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <DFComAPI.h>
#include <AEErrorDef.h>


int main(int /*argc*/, char** /*argv[]*/)
{
	int comNum = 1, busNum = 254;
	char deviceIp[255] = "10.6.100.107";
	int port = 8000;

	printf("Opening Socket\n");
	if(! DFCComOpenIV(comNum, busNum, 3, deviceIp, port, 3000)) {
		printf("Error opening Socket\n");
		return -1;
	}

	int error = 0, serial = 0;
	printf("Getting Serial\n");
	printf("call DFCGetSeriennummer(comNum=%d, busNum=%d, &error=%d, &serial=%d)\n", comNum, busNum, error, serial);
	if(! DFCGetSeriennummer(comNum, busNum, &error, &serial)) {
		// DFCGetErrorText(g_CommonInfo.ucComNum, error, 0, g_CommonInfo.szErrorMsg, 254);

		printf("Error getting Serial\n");
		return -2;
	}

	printf("return DFCGetSeriennummer(comNum=%d, busNum=%d, &error=%d, &serial=%d)\n", comNum, busNum, error, serial);
	printf("Serial = %d\n", serial);

	printf("Closing Socket\n");
	DFCComClose(comNum);
	return 0;
}


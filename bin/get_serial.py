#!/usr/bin/python3

import sys, os

sys.path.insert(0, os.path.abspath(os.path.join(__file__, "../../module/")))
import pydfcom

print("connect")
pydfcom.connect(
	"10.6.100.107",  # host
	8000,            # port
	3000)            # read-timeout

print("get_serial")
print(pydfcom.get_serial());
pydfcom.disconnect()

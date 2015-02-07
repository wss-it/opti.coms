#!/usr/bin/python3

import sys, os
from datetime import datetime

sys.path.insert(0, os.path.abspath(os.path.join(__file__, "../../../module/")))
import pydfcom

print("connect()")
pydfcom.connect(
	"10.6.100.107",  # host
	8000,            # port
	3000)            # read-timeout


print("get_datetime() = %s" %
	pydfcom.get_datetime());  # datetime object

now = datetime.today()

print("set_datetime(%s)" % now);
pydfcom.set_datetime(now);

print("disconnect()")
pydfcom.disconnect()

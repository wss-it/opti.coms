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

identities = [
	{'ID': '0030074700811', 'Name': 'Peter Körner'},
	{'ID': '0030074229747', 'Name': 'Jasmin Körner'},
]

#desc = [
#	# List 0
#	[
#		{'fieldname': 'ID', 'type': 4, 'size': 14},
#		{'fieldname': 'Name', 'type': 4, 'size': 31}
#	]
#]
desc = pydfcom.read_list_descriptions();
print("read_list_descriptions() = %s" %
	desc);

print("push_list()")
pydfcom.push_list(
	0,           # List-ID
	desc[0],     # List-Description
	identities)  # Data

print("disconnect()")
pydfcom.disconnect()

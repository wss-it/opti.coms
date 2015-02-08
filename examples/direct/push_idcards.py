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
	# ID              Name
	['0030074700811', 'Peter Koerner'],
	['0030074229747', 'Dennis Sepeur'],
]

pydfcom.push_list(
	0,           # List-ID
	[13, 30,],   # Field-Lengths
	identities)  # Data

print("disconnect()")
pydfcom.disconnect()

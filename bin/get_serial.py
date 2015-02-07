#!/usr/bin/python3

import sys, os

sys.path.insert(0, os.path.abspath(os.path.join(__file__, "../../module/")))
import pydfcom

print(pydfcom.get_serial("10.6.100.107", 8000));

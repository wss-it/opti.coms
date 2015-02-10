# -*- coding: utf8 -*-
from distutils.core import setup, Extension

pydfcom = Extension('pydfcom',
                    libraries = ['boost_python-py34'],
                    library_dirs = ['/usr/lib'],
                    include_dirs = ['../vendor/DFCom/include', '/usr/include'],
                    sources = ['pydfcom.cc'],
                    extra_objects=['../vendor/DFCom/ar/DFCom.a'])

setup (name = 'DataFox Communications',
       url='https://github.com/wss-it/opti.coms',
       author='Peter Körner',
       author_email='peter@mazdermind.de',
       version = '1.0',
       description = 'DataFox Communication Library Bindings',
       ext_modules = [pydfcom])

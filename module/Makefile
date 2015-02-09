#------------------------------------------------------------------------------
#
#  osm history importer makefile
#
#------------------------------------------------------------------------------

#CXX = g++
CXX = clang++

CXXFLAGS = -g -O3 -Wall -Wextra -pedantic
CXXFLAGS += `getconf LFS_CFLAGS`
#CXXFLAGS += -Wredundant-decls -Wdisabled-optimization
#CXXFLAGS += -Wpadded -Winline

ARS = ../vendor/DFCom/ar/DFCom.a
DFCOM_INCLUDE = ../vendor/DFCom/include/

# location of the Python header files
PYTHON_INCLUDE = /usr/include/python3.4

# location of the Boost Python include files and library
BOOST_INC = /usr/include
BOOST_LIB = /usr/lib

# compile mesh classes
TARGET = pydfcom

all: $(TARGET).so

$(TARGET).so: $(TARGET).o
	$(CXX) -shared -Wl,--export-dynamic $(TARGET).o $(ARS) -L$(BOOST_LIB) -lboost_python-py34 -L/usr/lib/python3.4/config -lpython3.4m -o $(TARGET).so

$(TARGET).o: $(TARGET).cc
	$(CXX) -g -I$(DFCOM_INCLUDE) -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -c $(TARGET).cc

clean:
	rm -f $(TARGET).o $(TARGET).so
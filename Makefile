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

# path to includes
CXXFLAGS += -I./DFCom/include/

# compile & link against expat to have xml reading support
LDFLAGS += -lpthread

ARS += ./DFCom/ar/DFCom.a

.PHONY: all clean install

all: opticoms

opticoms: opticoms.o
	$(CXX) -o $@ $< $(ARS) $(LDFLAGS)

install:
	install -m 755 -g root -o root -d $(DESTDIR)/usr/bin
	install -m 755 -g root -o root opticoms $(DESTDIR)/usr/bin/opticoms

clean:
	rm -f *.o opticoms

# This will try to compile each include file on its own to detect missing
# #include directives. Note that if this reports [OK], it is not enough
# to be sure it will compile in production code. But if it reports [FAILED]
# we know we are missing something.
check-includes:
	echo "check includes report:" >check-includes-report; \
	for FILE in *.h *.cc; do \
		echo "$${FILE}:" >>check-includes-report; \
		echo -n "$${FILE} "; \
		if `$(CXX) -I include $${FILE} 2>>check-includes-report`; then \
			echo "[OK]"; \
		else \
			echo "[FAILED]"; \
		fi; \
		rm -f $${FILE}.gch; \
	done

indent:
	astyle --style=java --indent-namespaces --indent-switches --pad-header --suffix=none --recursive include/\*.hpp *.cc

#deb:
#	debuild -I -us -uc

#deb-clean:
#	debuild clean

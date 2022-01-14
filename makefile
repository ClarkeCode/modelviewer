COMPILER = gcc
CFLAGS = -ansi -Wall -std=c11 -g

PYTHON_CFLAGS = -I/usr/include/python3.7m -I/usr/include/python3.7m  -Wno-unused-result -Wsign-compare -g -fdebug-prefix-map=/build/python3.7-3.7.3=. -specs=/usr/share/dpkg/no-pie-compile.specs -fstack-protector -Wformat -Werror=format-security  -DNDEBUG -g -fwrapv -O3 -Wall -fPIC
PYTHON_LDFLAGS = -L/usr/lib/python3.7/config-3.7m-x86_64-linux-gnu -L/usr/lib -lpython3.7m -lcrypt -lpthread -ldl  -lutil -lm  -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions

COMPILE_DEFINITIONS = #-DDISPLAY_OBJ_CONTENT -DDISPLAY_PPM_CONTENT

VG = valgrind --leak-check=yes --track-origins=yes
SUPERVG = valgrind --leak-check=full --track-origins=yes -v --log-file="VGoutput.txt"

a1: main.o customFunctionsA1.o customUtilities.o embeddedPythonA2.o interopFunctionsA3.o readppm.py
	$(COMPILER) $(CFLAGS) $(PYTHON_LDFLAGS) main.o customFunctionsA1.o embeddedPythonA2.o interopFunctionsA3.o customUtilities.o -o a1

main.o: main.c makefile
	$(COMPILER) $(CFLAGS) $(COMPILE_DEFINITIONS) main.c -o main.o -c

customFunctionsA1.o: customFunctionsA1.c customFunctionsA1.h
	$(COMPILER) $(CFLAGS) customFunctionsA1.c -o customFunctionsA1.o -c

embeddedPythonA2.o: embeddedPythonA2.c embeddedPythonA2.h
	$(COMPILER) $(CFLAGS) $(PYTHON_CFLAGS) embeddedPythonA2.c -o embeddedPythonA2.o -c

customUtilities.o: customUtilities.c customUtilities.h
	$(COMPILER) $(CFLAGS) customUtilities.c -o customUtilities.o -c

interopFunctionsA3.o: interopFunctionsA3.c interopFunctionsA3.h dbFunctionsA4.h
	$(COMPILER) $(CFLAGS) interopFunctionsA3.c -o interopFunctionsA3.o -c

WEBSERV = /srv/rclark12/
package: a1
	tar cf package.tar a1 *.php *.js *.css *.py
	mv package.tar $(WEBSERV)package.tar

clean:
	rm -f ./a1
	rm -f *.o
	rm -f loaddata.js
	rm -rf __pycache__
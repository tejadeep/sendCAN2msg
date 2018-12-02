CC=g++
CFLAGS=-Wall -fno-exceptions -g -pedantic -O0 -I.
LDFLAGS=-l wiringPi
EXEC=sendRPM

all:	$(EXEC)

sendRPM: readIni.o MCP2515.o canfoncts.o main.o sllist.o
	$(CC) -o $@ readIni.o MCP2515.o canfoncts.o sllist.o main.o $(LDFLAGS)

readIni.o: readIni.cpp
	$(CC) -o $@ -c readIni.cpp $(CFLAGS)

MCP2515.o: MCP2515.cpp readIni.h
	$(CC) -o $@ -c MCP2515.cpp $(CFLAGS)

canfoncts.o: canfoncts.cpp
	$(CC) -o $@ -c canfoncts.cpp $(CFLAGS)

sllist.o: sllist.cpp
	$(CC) -o $@ -c sllist.cpp $(CFLAGS)

main.o: main.cpp canfoncts.h MCP2515.h readIni.h sllist.h
	$(CC) -o $@ -c main.cpp $(CFLAGS)

clean:
	rm -f *.o core
	
mrproper: clean
	rm -f $(EXEC)

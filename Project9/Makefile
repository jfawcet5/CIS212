CFLAGS=-W -Wall -I/usr/local/include -g
LDFLAGS=-L/usr/local/lib -g
PROGRAMS=longtest stringtest
OBJECTS=set.o sort.o
LIBRARIES=-lADTs

all: $(PROGRAMS)

longtest: longtest.o $(OBJECTS)
	gcc $(LDFLAGS) -o longtest $^ $(LIBRARIES)

stringtest: stringtest.o $(OBJECTS)
	gcc $(LDFLAGS) -o stringtest $^ $(LIBRARIES)

longtest.o: longtest.c set.h
stringtest.o: stringtest.c set.h
set.o: set.c set.h
sort.o: sort.c sort.h

clean:
	rm -f $(PROGRAMS) longtest.o stringtest.o $(OBJECTS)

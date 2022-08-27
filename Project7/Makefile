CFLAGS= -W -Wall
OBJECTS= exerciseQueue.o
LDFLAGS= -L/usr/local/lib
LIBRARIES= -lADTs
PROGRAMS= exerciseQueue exerciseQueue2

all: $(PROGRAMS)

exerciseQueue: $(OBJECTS)
	gcc $(LDFLAGS) -o exerciseQueue $^ $(LIBRARIES)

exerciseQueue2: exerciseQueue.o queue.o
	gcc $(LDFLAGS) -o exerciseQueue2 $^ $(LIBRARIES)

queue.o: queue.c
exerciseQueue.o: exerciseQueue.c

clean:
	rm -f exerciseQueue.o queue.o $(PROGRAMS)

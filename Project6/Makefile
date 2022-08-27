CFLAGS= -W -Wall -I/usr/local/include
OBJECTS= exerciseStack.o balanceCheck.o
PROGRAMS= exerciseStack balanceCheck
LDFLAGS= -L/usr/local/lib
LIBRARIES= -lADTs

all: $(PROGRAMS)

exerciseStack: exerciseStack.o
	gcc -o exerciseStack $^ $(LIBRARIES)

balanceCheck: balanceCheck.o
	gcc -o balanceCheck $^ $(LIBRARIES)

exerciseStack.o: exerciseStack.c

balanceCheck.o: balanceCheck.c 

clean:
	rm -f $(PROGRAMS) $(OBJECTS)

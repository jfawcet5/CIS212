CLFAGS=-W -Wall
OBJECTS=wordperline.o arraylist.o stringADT.o iterator.o

wordperline: $(OBJECTS)
	gcc -o wordperline $(OBJECTS)

wordperline.o: wordperline.c
arraylist.o: arraylist.c arraylist.h
stringADT.o: stringADT.c stringADT.h
iterator.o: iterator.c iterator.h

clean:
	rm -f wordperline $(OBJECTS)

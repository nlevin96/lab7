
all: hexeditplus

hexeditplus: hexeditplus.o 
	gcc -g -Wall -o hexeditplus hexeditplus.o
	
hexeditplus.o: task1.c
	gcc -g -Wall -c -o hexeditplus.o task1.c 


.PHONY: clean

clean: 
	rm -f *.o hexeditplus

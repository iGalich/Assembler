FLAGS = gcc -c -Wall -ansi -pedantic

ASSEMBLER_OBJECTS = assembler.o filefunc.o utility.o linkedlist.o

HEADERS = assembler.h linkedlist.h const.h

assembler: $(ASSEMBLER_OBJECTS)
	gcc -g -ansi -Wall -ansi -pedantic $(ASSEMBLER_OBJECTS) -o assembler

assembler.o: assembler.c $(HEADERS)
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o

filefunc.o: filefunc.c $(HEADERS)
	gcc -c -Wall -ansi -pedantic filefunc.c -o filefunc.o

utility.o: utility.c $(HEADERS)
	gcc -c -Wall -ansi -pedantic utility.c -o utility.o

linkedlist.o: linkedlist.c $(HEADERS)
	gcc -c -Wall -ansi -pedantic linkedlist.c -o linkedlist.o

remove:
	rm *.o
	rm *.am
	rm *.ent
	rm *.ext
	rm *.ob
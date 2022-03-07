FLAGS = gcc -c -Wall -ansi -pedantic

ASSEMBLER_OBJECTS = assembler.o filefunc.o utility.o linkedlist.o

HEADERS = assembler.h linkedlist.h const.h

assembler: $(ASSEMBLER_OBJECTS)
	gcc -g -ansi -Wall -ansi -pedantic $(ASSEMBLER_OBJECTS) -o assembler

assembler.o: assembler.c $(HEADERS)
	$(FLAGS) assembler.c -o assembler.o

filefunc.o: filefunc.c $(HEADERS)
	$(FLAGS) filefunc.c -o filefunc.o

utility.o: utility.c $(HEADERS)
	$(FLAGS) utility.c -o utility.o

linkedlist.o: linkedlist.c $(HEADERS)
	$(FLAGS) linkedlist.c -o linkedlist.o

remove:
	rm *.o
	rm *.am
	rm *.ent
	rm *.ext
	rm *.ob
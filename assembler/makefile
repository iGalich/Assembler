FLAGS = gcc -c -Wall -ansi -pedantic

ASSEMBLER_OBJECTS = assembler.o filefunc.o utility.o linkedlist.o symbollinkedlist.o datalinkedlist.o firstpass.o stringfunc.o secondpass.o addresslinkedlist.o finalfiles.o

HEADERS = assembler.h linkedlist.h const.h symbollinkedlist.h addresslinkedlist.h

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

symbollinkedlist.o: symbollinkedlist.c $(HEADERS)
	$(FLAGS) symbollinkedlist.c -o symbollinkedlist.o

datalinkedlist.o: datalinkedlist.c $(HEADERS)
	$(FLAGS) datalinkedlist.c -o datalinkedlist.o

firstpass.o: firstpass.c $(HEADERS)
	$(FLAGS) firstpass.c -o firstpass.o -lb

stringfunc.o: stringfunc.c $(HEADERS)
	$(FLAGS) stringfunc.c -o stringfunc.o

secondpass.o: secondpass.c $(HEADERS)
	$(FLAGS) secondpass.c -o secondpass.o

addresslinkedlist.o: addresslinkedlist.c $(HEADERS)
	$(FLAGS) addresslinkedlist.c -o addresslinkedlist.o

finalfiles.o: finalfiles.c $(HEADERS)
	$(FLAGS) finalfiles.c -o finalfiles.o

remove:
	rm *.o
	rm *.am
	rm *.ent
	rm *.ext
	rm *.ob
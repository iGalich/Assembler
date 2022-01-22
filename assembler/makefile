assembler: assembler.o filefunc.o otherfunc.o
	gcc -g -ansi -Wall -ansi -pedantic assembler.o filefunc.o otherfunc.o -o assembler

assembler.o: assembler.c assembler.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o

filefunc.o: filefunc.c assembler.h
	gcc -c -Wall -ansi -pedantic filefunc.c -o filefunc.o

otherfunc.o: otherfunc.c assembler.h
	gcc -c -Wall -ansi -pedantic otherfunc.c -o otherfunc.o
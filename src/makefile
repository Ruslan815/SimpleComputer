.PHONY: clean

all: SimpleComputer.exe

SimpleComputer.exe: libdisplayTerm.a libmyReadKey.a libmyBigChars.a libmyTerm.a libmySimpleComputer.a libCPU.a main.o 
	gcc -Wall -Werror main.o -L. -ldisplayTerm -lmyReadKey -lmyBigChars -lmyTerm -lmySimpleComputer -lCPU -o SimpleComputer.exe -lm

libmySimpleComputer.a: mySimpleComputer.o
	ar rc libmySimpleComputer.a mySimpleComputer.o
	ranlib libmySimpleComputer.a

libmyTerm.a: myTerm.o
	ar rc libmyTerm.a myTerm.o
	ranlib libmyTerm.a

libmyBigChars.a: myBigChars.o
	ar rc libmyBigChars.a myBigChars.o
	ranlib libmyBigChars.a

libmyReadKey.a: myReadKey.o
	ar rc libmyReadKey.a myReadKey.o
	ranlib libmyReadKey.a

libdisplayTerm.a: displayTerm.o
	ar rc libdisplayTerm.a displayTerm.o
	ranlib libdisplayTerm.a

libCPU.a: CPU.o
	ar rc libCPU.a CPU.o 
	ranlib libCPU.a

mySimpleComputer.o: mySimpleComputer.c
	gcc -Wall -Werror -c mySimpleComputer.c

myTerm.o: myTerm.c
	gcc -Wall -Werror -c myTerm.c

myBigChars.o: myBigChars.c
	gcc -Wall -Werror -c myBigChars.c

myReadKey.o: myReadKey.c
	gcc -Wall -Werror -c myReadKey.c

displayTerm.o: displayTerm.c
	gcc -Wall -Werror -c displayTerm.c

CPU.o: CPU.c 
	gcc -Wall -Werror -c CPU.c 

main.o: main.c
	gcc -Wall -Werror -c main.c

translatorAsm: TranslatorAssembly.exe

TranslatorAssembly.exe: libmySimpleComputer.a libmyReadKey.a mainAssembly.o
	gcc -Wall -Werror mainAssembly.o -L. -lmySimpleComputer -lmyReadKey -o TranslatorAssembly.exe -lm

mainAssembly.o: mainAssembly.c
	gcc -Wall -Werror -c mainAssembly.c

translatorBas: TranslatorBasic.exe

TranslatorBasic.exe: libmySimpleComputer.a libmyReadKey.a mainBasic.o
	gcc -Wall -Werror mainBasic.o -L. -lmySimpleComputer -lmyReadKey -o TranslatorBasic.exe -lm

mainBasic.o: mainBasic.c
	gcc -Wall -Werror -c mainBasic.c

clean:
	rm -rf *.o *.exe *.a

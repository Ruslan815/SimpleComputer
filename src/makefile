.PHONY: clean

all: SimpleComputer.exe

SimpleComputer.exe: libdisplayTerm.a libmyReadKey.a libmyBigChars.a libmyTerm.a libmySimpleComputer.a main.o 
	gcc -Wall -Werror main.o -L. -ldisplayTerm -lmyReadKey -lmyBigChars -lmyTerm -lmySimpleComputer -o SimpleComputer.exe -lm

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

main.o: main.c
	gcc -Wall -Werror -c main.c

translator: TranslatorAssembly.exe

TranslatorAssembly.exe: libmySimpleComputer.a libtranslatorAssembly.a libmyReadKey.a mainTranslator.o
	gcc -Wall -Werror mainTranslator.o -L. -ltranslatorAssembly -lmySimpleComputer -lmyReadKey -o TranslatorAssembly.exe -lm

mainTranslator.o: mainTranslator.c
	gcc -Wall -Werror -c mainTranslator.c

libtranslatorAssembly.a: translatorAssembly.o
	ar rc libtranslatorAssembly.a translatorAssembly.o
	ranlib libtranslatorAssembly.a

translatorAssembly.o: translatorAssembly.c
	gcc -Wall -Werror -c translatorAssembly.c

clean:
	rm -rf *.o *.exe *.a

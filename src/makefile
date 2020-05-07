.PHONY: clean

all: LABA6.exe

LABA6.exe: libdisplayTerm.a libmyReadKey.a libmyBigChars.a libmyTerm.a libmySimpleComputer.a mainFileLABA6.o
	gcc -Wall -Werror mainFileLABA6.o -L. -ldisplayTerm -lmyReadKey -lmyBigChars -lmyTerm -lmySimpleComputer -o LABA6.exe -lm

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

mainFileLABA6.o: mainFileLABA6.c
	gcc -Wall -Werror -c mainFileLABA6.c

clean:
	rm -rf *.o *.a *.exe

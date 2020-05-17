.PHONY: clean

all: main.exe

main.exe: main.o
	gcc -Wall -Werror main.o -o main.exe

main.o: main.c 
	gcc -Wall -Werror -c main.c 

clean:
	rm -rf *.o *.exe
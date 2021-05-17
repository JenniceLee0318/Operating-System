all: main

main.o: main.c
	gcc -c main.c

main: main.o
	gcc -o main main.o 

clean:
	rm -f main *.o && clear
all: UnixLs

UnixLs.o: UnixLs.c
	gcc -c -g -Wall UnixLs.c

UnixLs: UnixLs.o
	gcc -o UnixLs UnixLs.o

clean:
	rm -f UnixLs *.o && clear
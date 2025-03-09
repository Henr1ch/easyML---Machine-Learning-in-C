morganNicholasA1: morganNicholasA1.o morganNicholasA1Main.o
	gcc morganNicholasA1.o morganNicholasA1Main.c -o morganNicholasA1 -lm
morganNicholasA1.o: morganNicholasA1.c givenA1.h
	gcc -Wall -std=c99 -c morganNicholasA1.c
morganNicholasA1Main.o: morganNicholasA1Main.c givenA1.h
	gcc -Wall -std=c99 -c morganNicholasA1Main.c
clean:
	rm *.o morganNicholasA1

motor: clean maq.o pilha.o arena.o
	gcc -std=c99 -g -o motor maq.o pilha.o arena.o

#motor.o: motor.c
#	gcc -g -c motor.c

maq.o: maq.c
	gcc -std=c99 -g -c maq.c

pilha.o: pilha.c
	gcc -std=c99 -g -c pilha.c

arena.o: arena.c
	gcc -std=c99 -g -c -lm arena.c

clean:
	rm -f motor *.o

motor: clean maq.o pilha.o arena.o
	gcc -g -o motor maq.o pilha.o arena.o

maq.o: maq.c
	gcc -g -c maq.c

pilha.o: pilha.c
	gcc -g -c pilha.c

arena.o: arena.c
	gcc -g -c -lm arena.c

clean:
	rm -f motor *.o

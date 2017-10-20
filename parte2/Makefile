#motor: clean maq.o pilha.o motor.o
#	gcc -g -o motor maq.o pilha.o motor.o 
#
#maq.o: maq.c
#	gcc -g -c maq.c
#
#pilha.o: pilha.c
#	gcc -g -c pilha.c
#
#motor.o: motor.c
#	gcc -g -c motor.c
#
#clean:
#	rm -f motor *.o

motor: arena.o
	gcc -g -o motor arena.o 

arena.o: arena.c
	gcc -g -c -lm arena.c
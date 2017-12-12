motor: clean maq.o pilha.o arena.o SisMov.o SisAtq.o SisRec.o SisDep.o
	gcc -std=c99 -g -o motor maq.o pilha.o arena.o SisMov.o SisAtq.o SisRec.o SisDep.o
	./motor > a.o
	cat a.o
	python3 apres.py < a.o

SisAtq.o: SisAtq.c
	gcc -std=c99 -g -c SisAtq.c

SisRec.o: SisRec.c
	gcc -std=c99 -g -c SisRec.c

SisDep.o: SisDep.c
	gcc -std=c99 -g -c SisDep.c

SisMov.o: SisMov.c
	gcc -std=c99 -g -c SisMov.c

maq.o: maq.c
	gcc -std=c99 -g -c maq.c

pilha.o: pilha.c
	gcc -std=c99 -g -c pilha.c

arena.o: arena.c
	gcc -std=c99 -g -c -lm arena.c

clean:
	rm -f motor *.o

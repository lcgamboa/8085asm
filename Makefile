CC=gcc
FLAGS= -ggdb -Wall


all: 8085asm.c 8085dasm.c hexfile.c
	$(CC) $(FLAGS) 8085asm.c -o 8085asm
	$(CC) $(FLAGS) hexfile.c -c
	$(CC) $(FLAGS) 8085dasm.c hexfile.o -o 8085dasm

clean:
	rm -f 8085asm 8085dasm *.hex *.map *.o
	rm -f examples/*.hex examples/*.map

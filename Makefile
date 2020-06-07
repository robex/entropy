ent: ent.c argparse.o
	gcc -o ent ent.c argparse.o -Wall -lm

argparse.o: argparse.c argparse.h
	gcc -c -o argparse.o argparse.c -Wall

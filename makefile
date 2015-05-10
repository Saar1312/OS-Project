all: procesos.c

	gcc header.h persona.c procesos.c -o procesos


clear:

	rm procesos

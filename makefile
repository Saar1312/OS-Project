all: procesos.c

	gcc -g header.h persona.c map.c reduce.c pares.c procesos.c -o friendfindP


clear:

	rm procesos

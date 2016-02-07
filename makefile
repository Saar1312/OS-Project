FILES=header.h persona.c map.c reduce.c pares.c procesos.c 
FLAGS=-g -o
make: $(FILES)
	gcc $(FLAGS) friendfindP $(FILES)
clean:
	rm -f friendfindP
#holaaaaa

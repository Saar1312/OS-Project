///////////////////////////////////////////////////////////////////////////////////////////
//    Universidad Simón Bolívar                                                          //
//    Dpto. de Computación y Tecnología de la Información                                //
//    CI3825 - Sistemas de Operación                                                     //
//    Abril - Julio 2015                                                                 //
//                                                                                       //
//    Integrantes:                                                                       //
//        Andrea Centeno, 10-10138                                                       //
//        Samuel Arleo R, 10-10969                                                       //
///////////////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

//Lista enlazada que almacena una persona y a sus amistades
typedef struct Persona{
	char *nombre;
	struct Persona *amigos;
} Persona;

typedef struct Par{
	char *amigo1;
	char *amigo2;
	Persona *amigos;
} Par;

///////////////////////////// Calcula el numero de lineas del archivo /////////////////////////////
int numeroDeLineas(FILE *f, char caracter){

	int lineas = 0;
		
	while ((caracter=fgetc(f)) != EOF){
		if (caracter == '\n'){
			lineas++;
		}
	}
	return lineas;
}

////////////////////////////// Lee una linea y devuelve una Persona //////////////////////////////
void leerLineaPersona(FILE *archivo,Persona *p){

	char *str = malloc(sizeof(char)*15);
    fscanf(archivo, "%s" ,str);
    p->nombre = str; //le asigna el nombre a la persona

    char caracter = 1;
    Persona *aux = p;

    //lee flecha
    char flecha[3];
    fscanf(archivo, "%s" ,flecha);
    int j=0;

    while (caracter != '\n'){

       	char *strn = malloc(sizeof(char)*15);
	    fscanf(archivo, "%s" ,strn);

       	Persona *person = malloc(sizeof(Persona));
		person->nombre = strn;
		aux->amigos = person;
		aux = person;
		aux->amigos= NULL;
		caracter = fgetc(archivo);
		
    }
}

////////////////////////////////// Imprimer estructura persona //////////////////////////////////
void imprimirPersona(Persona *p){
	printf("%s ->",p->nombre);
	Persona *aux = p->amigos;
	while(aux!=NULL){
		printf(" %s",aux->nombre);
		aux = aux->amigos;
	}
	printf("\n");
}

///////////////////////////////////////////// MAIN /////////////////////////////////////////////
int main(int argc,char *argv[]){

	printf("\n");

	FILE *f = fopen(argv[2],"r");
	char caracter = fgetc(f);

	int lineas = numeroDeLineas(f, caracter);

	rewind(f);	

	//lee una linea y crea la estructura persona
    Persona *p = malloc(sizeof(Persona));
	leerLineaPersona(f,p);

	imprimirPersona(p); //PRUEBA

	/*
	int num_procesos = atoi(argv[1]);
	int lineas_proceso = lineas/num_procesos;

	if (num_procesos >= 1){ //mas lineas que procesos
		if (lineas % num_procesos != 0){
			lineas_proceso++;
		}
	}
	else{ // mas procesos que lineas
		lineas_proceso = 1;
	}
	int i,status;
	pid_t childpid;
	for(i=0; i < num_procesos ;i++){
		//ver man de fork()
		if ((childpid = fork()) < 0){
			perror("fork:");
			exit(1);
		}
		else if(childpid == 0){
			rewind(f);			
			printf("i: %d\t lineas_proceso: %d\n\n\n",i,lineas_proceso);
			int contador = 0;
			while ((caracter = fgetc(f)) != EOF){	
				if ((i*lineas_proceso <= contador) && (contador < (i+1)*lineas_proceso)){
					printf("%c",caracter);
				}
				if (caracter == '\n'){
					contador++;
				}
			}
			exit(0);
		}
			// esto le corresponde  a la funcion Map
	}for (i=0; i<=num_procesos;i++){
		wait(&status);
	}
	*/
	return 1;	
}

void Map(){


}
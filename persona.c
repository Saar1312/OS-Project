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

#include "header.h"

//// FUNCIONES RELACIONADAS A LA ESTRUCTURA PERSONA ////

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

//////////////////////////// Liberar espacio de estructura Persona ////////////////////////////
void liberarPersona(Persona *p){
	Persona *aux = p;
	Persona *sig;
	while(aux!=NULL){
		sig = aux->amigos;
		free(aux->nombre);
		free(aux);
		aux = sig;
	}
	free(p);
}
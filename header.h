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

//// ESTRUCTURAS ////

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
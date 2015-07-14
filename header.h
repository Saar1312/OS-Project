///////////////////////////////////////////////////////////////////////////////////////////
//    Universidad Simón Bolívar                                                          //
//    Dpto. de Computación y Tecnología de la Información                                //
//    CI3825 - Sistemas de Operación                                                     //
//    Abril - Julio 2015                             	                                 //
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

//Lista enlazada que almacena cada par de amigos y sus amigos
typedef struct Par{
	char *amigo1;
	char *amigo2;
	Persona *amigos1;
	Persona *amigos2;
	struct Par *sig;
} Par;

////// FIRMAS //////
int numeroDeLineas(char arch[]);
int tareasPorProceso(int lineasArchivo,int numProcesos);
void alcanzarLinea(FILE *f,int linea);
Persona *crearArregloPersonas(FILE *f, int n);
void distribuirMap(char *archivo,int lineasArchivo,int numProcesos,int lineasProceso);
void recibirPIDS(int numProcesos,pid_t PIDS[]);
void escribirResultado(FILE *f,pid_t PIDS[],int numPares);

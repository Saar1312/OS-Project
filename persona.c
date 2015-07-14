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
// Parametros:
//       FILE *archivo archivo a leer
//       Persona *p es donde se almacenara los datos leidos
void leerLineaPersona(FILE *archivo,Persona *p){ 

	char caracter,caracter2 = 0;
	char *nombre = malloc(sizeof(char)*30);
	int i = 0; //esAmigo: booleano que indica si el nombre leido es el nombre de la persona o es un amigo de la lista
	int j = 0;
	int esAmigo = 0;
	int contador = 0;
	char *nombre2;
	Persona *aux2 = p; //aux2 apunta al final de la lista enlazada para agregar nuevos elementos
	while (1){
		caracter = fgetc(archivo);
		if ((caracter != 32) && (caracter != EOF) && (caracter != '\n')){ //Si es distinto del espacio va formando el string
			if (i < 30){
				nombre[i] = caracter;
			}
			else{
				nombre = realloc(nombre,30);
				if (nombre != NULL){
					nombre[i] = caracter;
				}
				else{
					printf("Error de asignacion de memoria\n");
					exit(-2); //Si el hijo termina mal el padre debe saber que no va a buscar en su archivo
				}
			}
			i++;
		}		
		else{ 

			nombre2 = malloc(sizeof(char)*i); //filtra el string creado y lo coloca del tamano justo
			for (j = 0; j<i; j++){
				caracter2 = nombre[j];
				nombre2[j] = caracter2;
			}
			if(contador != 1){
				if (!esAmigo){ //la primera vez el nombre leido corresponde a la persona
					p->nombre = malloc(sizeof(char)*i);
					strcpy(p->nombre,nombre2);
					p->nombre = nombre2;
					esAmigo = 1;
				}
				else{
					Persona *aux = malloc(sizeof(Persona));
					aux->nombre = malloc(sizeof(char)*i);
					strcpy(aux->nombre,nombre2);
					aux2->amigos = aux;
					aux2 = aux;
					aux2->amigos = NULL;
				}
			}
			
			contador++;
			i = 0;
		}
		if((caracter == '\n') || (caracter == EOF)){
			break;
		}
	}
	free(nombre2);
	free(nombre); //libera la var auxiliar que guardaba los nombres
}


////////////////////////////////// Imprimer estructura persona //////////////////////////////////
// Parametros:
//       Persona *p persona a imprimir
void imprimirPersona(Persona *p){
	
	Persona *aux = p->amigos;
	while(aux!=NULL){
		printf("%s ",aux->nombre);
		aux = aux->amigos;
	}
	printf("\n");
}

//////////////////////////// Liberar espacio de estructura Persona ////////////////////////////
// Parametros:
//       Persona *p persona a liberar
void liberarPersona(Persona *p){
	free(p->nombre);
	Persona *aux = p->amigos;
	Persona *sig;
	while(aux!=NULL){
		sig = aux->amigos;
		free(aux->nombre);
		free(aux);
		aux = sig;
	}
}

/////////////////////////// Libera la memoria del arreglo de Personas ///////////////////////////
// Parametros:
//       int n cantidad de personas en el arreglo
//       Persona *arreglo arreglo de personas a liberar
void liberarArregloPersonas(Persona *arreglo, int n){
	int i=n;
	while(i>0){
		liberarPersona(arreglo + i -1);
		i--;
	}
	free(arreglo);
}
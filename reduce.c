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

////////////////////////////// Funcion Reduce//////////////////////////////
// Parametros: 
//       Par *p lista depares a reducir
//       char archivo[] nombre del archivo a escribir

void reduce(Par *p,char archivo[]){
	FILE *f = fopen(archivo,"a+");
	int entrar = 0;
	if (p->amigos2 == NULL){
		fprintf(f,"(%s %s) -> -None-\n",p->amigo1,p->amigo2);
	}
	if (p->amigos2 != NULL){
		fprintf(f,"(%s %s) -> ",p->amigo1,p->amigo2);
		Persona *aux = p->amigos1;
		Persona *aux1 = p->amigos2;
		while (aux != NULL){
			while(aux1 != NULL){
				if (strcmp(aux->nombre,aux1->nombre) == 0){
					fprintf(f,"%s ",aux->nombre);
					entrar = 1;
				}
				aux1 = aux1->amigos;
			}
			aux1 = p->amigos2;
			aux = aux->amigos;
		}
		if (!entrar){
			fprintf(f,"-None-");
		}
		fprintf(f,"\n");
	}
	fclose(f);
}
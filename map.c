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

///////////////////// Verifica si el par ya se encuentra en el archivo///////////////////////
// Parametros: 
//       char persona1[], persona2[] nombres de las personas abuscar en los pares
//       char arch[] nombre del archivo a leer
char verificaSiYaEsta(char arch[],char persona1[],char persona2[]){
	char esta = 0; //dice si el par esta en el archivo

	//tamanio del string a buscar
	int tam = strlen(persona1) + 2 + strlen(persona2);
	//crea un string con el nombre de ambas personas a buscar
	char buscar[tam];
	buscar[0]='\0';
	strcat(buscar, persona2);
	strcat(buscar, " ");
	strcat(buscar, persona1);

	char nombres[tam]; //almacenara los nombres leidos del archivo
	char caracter=1;

	if(access(arch, 0 ) != -1){
		FILE *archivo = fopen(arch, "r");
    	while (!feof(archivo) && caracter!= EOF){
			fgets(nombres,tam,archivo);
			if(strcmp(nombres,buscar) == 0){
				esta = 1;
				break;
			}
			else{
				while(caracter != 10 && caracter!= EOF){
					caracter = fgetc(archivo);
				}
			}
		}
    	fclose(archivo);
	}
	return esta;
}

///////////////////////////// Agrega un par de amigos al archivo /////////////////////////////
// Parametros: 
//       Persona *p persona a la que se le hara map
//       char arch[] nombre del archivo a escribir
void escribePar(Persona *p, char arch[]){
	int MAX_buffer = 100;
	int contador = 0;
	//se crea un string con la lista de amigos
	char *amigos=malloc(sizeof(char)*MAX_buffer);
	amigos[0]='\0';
	Persona *aux = p->amigos;
	while (aux != NULL){
		//se reserva mas memoria en caso de ser necesario
		contador += strlen(aux->nombre);
		if(contador >= MAX_buffer){
			MAX_buffer *=2;
			amigos = realloc(amigos,MAX_buffer);
		}

		strcat(amigos, aux->nombre);
		aux = aux->amigos;
		if(aux!=NULL){
			strcat(amigos, " ");
		}
	}

	//se escriben todos los pares
	char esta = 0;
	Persona *amigo = p->amigos;
	FILE *archivoSalida;
	while (amigo != NULL){
		esta = verificaSiYaEsta(arch,p->nombre,amigo->nombre);
		archivoSalida = fopen(arch, "a+");
		if(esta == 1){ //si esta en el archivo escribe el par al reves 
			fprintf(archivoSalida, "%s %s -> %s", amigo->nombre,p->nombre,amigos);
		}
		else{//si no esta, lo escribe como esta en la estructura
			fprintf(archivoSalida, "%s %s -> %s", p->nombre,amigo->nombre,amigos);
		}
		amigo = amigo->amigos;
		if(amigo !=NULL){
			fprintf(archivoSalida, "\n");
		}
		fclose(archivoSalida);
		esta = 0;
	}
	free(amigos);
}

/////////////////////////////////////// Funcion Map ///////////////////////////////////////
// Parametros: 
//       Persona *lista lista de personas a las que se le hara map
//       char arch[] nombre del archivo a escribir
//       int n numero de lineas que lee el proceso
void map(char arch[], int n, Persona *lista){
	int i;
	
	for(i=0;i<n;i++){
		escribePar(lista+i, arch);
		if(i<n-1){
			FILE *archivoSalida = fopen(arch, "a+");
			fprintf(archivoSalida, "\n");
			fclose(archivoSalida);
		}
	}

}
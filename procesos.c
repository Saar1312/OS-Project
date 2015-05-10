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
	liberarPersona(p);

	int num_procesos = atoi(argv[1]); //numero de procesos
	int lineas_proceso = lineas/num_procesos; //lineas del archivo que le corresponden a cada proceso

	if (num_procesos >= 1){ //mas lineas que procesos
		if (lineas % num_procesos != 0){
			lineas_proceso++;
		}
	}
	else{ // mas procesos que lineas
		lineas_proceso = 1;
	}
	/*
	
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
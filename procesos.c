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

// Los procesos no comparten datos por lo que hay que leer el archivo con cada fork y llegar a la linea que le corresponde
// buscar como es el peo con *argv[] y por que se le pasa argv[1] a fopen en vez de *argv[1]
int main(int argc,char *argv[]){
	printf("%d\t%s\t%s\n\n",argc,argv[1],argv[2]);
	FILE *f = fopen(argv[1],"r");
	int lineas = 0;
	int caracter = fgetc(f);
		
	while ((caracter=fgetc(f)) != EOF){
		if (caracter == '\n'){
			lineas++;
		}
	}
	int num_procesos = atoi(argv[2]);
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
	return 1;	
}


void Map(){


}
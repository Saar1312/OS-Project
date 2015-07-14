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
int numeroDeLineas(char arch[]){
	FILE *f = fopen(arch,"r");
	
	int lineas = 0;
	char caracter = 1;
	while (caracter != EOF){
		caracter = fgetc(f);
		if ((caracter == '\n') || (caracter == EOF)){
			lineas++;
		}
	}
	return lineas;
	fclose(f);
}


///////////// Calcula el numero de tareas que le tocara realizar a cada proceso //////////
int tareasPorProceso(int lineasArchivo,int numProcesos){

	int lineas;
	if (lineasArchivo < numProcesos){
		lineas = 1;
	}
	else{
		if ((lineasArchivo % numProcesos) == 0){
			lineas = lineasArchivo/numProcesos;
		}
		else{
			lineas = (lineasArchivo/numProcesos) + 1;
		}
	}
	return lineas;
}

void alcanzarLinea(FILE *f,int linea){
	
	char caracter;
	int contador = 0;
	while (contador < linea){
		caracter = fgetc(f);
		if ((caracter == '\n') || (caracter == EOF)){
			contador++;
		}
	}
}


/////////////////////////// Crea un arreglo con n cantidad de personas ///////////////////////////
Persona *crearArregloPersonas(FILE *f, int n){ //MAX: Maximo numero de lineas que habra que leer. Sirve cuando el numero de
	Persona *lista = calloc(n, sizeof(Persona));		//lineas no es multiplo del numero de procesos, por lo que el ultimo proceso
	Persona *l = lista;									//va a querer leer una linea que no contiene el archivo
	int i;
	for(i=0;i<n;i++){
		leerLineaPersona(f,&lista[i]);
	}
	return l;
}

//////////////// DISTUBUYE ENTRE LOS PROCESOS LAS LINEAS QUE LE CORRESPONDE MAPEAR ////////////////
void distribuirMap(char *archivo,int lineasArchivo,int numProcesos,int lineasProceso){

	int i;
	pid_t childpid = 0;
	for (i = 0; i<numProcesos ;i++){
		if ((childpid = fork()) < 0){
			perror("No ha sido posible crear el hijo.");
			exit(1);	
		}
		if (childpid == 0){ // Si el numero de procesos es mayor que el numero de lineas, hay que crear procesos demas, que no haran nada)){
			if (i<lineasArchivo){
				FILE *f = fopen(archivo,"r");
				char PID[8];
				sprintf(PID,"%ld",(long)getpid());
				alcanzarLinea(f,i*lineasProceso);
				Persona *lista;
				if((lineasArchivo % numProcesos != 0) && (i == numProcesos - 1)){ //i == numProcesos para que solo entre con el ultimo
					int MAX = lineasProceso - (((lineasArchivo/numProcesos)+1)*numProcesos - lineasArchivo); //Si el numero de lineas no es
																											 //multiplo del nro de procesos
					if(MAX != 0){		// Puede pasar que sobre un proceso con 0 lineas, ejemplo 20 lineas 11 procesos											
						FILE *archivo = fopen(PID,"w+"); 
						lista = crearArregloPersonas(f, MAX);
						map(PID, lineasProceso, lista);
						liberarArregloPersonas(lista,lineasProceso);
						fclose(archivo);				 
					}
					else{
						exit(-2);
					}
				}
				else{
					FILE *archivo = fopen(PID,"w+"); 
					lista = crearArregloPersonas(f, lineasProceso);
					map(PID, lineasProceso, lista);
					liberarArregloPersonas(lista,lineasProceso);
					fclose(archivo);
				}
				fclose(f);
				exit(getpid());
			}
			else{ // Si el numero de procesos es mayor que el de las lineas no se creo que archivo, por lo que hay que decirle
				  // al padre que no trate de leer un archivo si el wait retorno -2
				exit(-2);
			}
		}
	}
}

///////////////// Guarda los PIDs de todos los hijos creados en un arreglo//////////////////
void recibirPIDS(int numProcesos,pid_t PIDS[]){
	int i,status;
	for (i=0; i<numProcesos ;i++){
		PIDS[i] = wait(&status);
		printf("HIJO: %d\n",PIDS[i]);
	} 
}

///////////////// Guarda los PIDs de todos los hijos creados en un arreglo//////////////////
void escribirArchivoFinal(int numProcesos,pid_t PIDS[], char arch[]){
	int i;
	char caracter;
	FILE *entrada;
	FILE *salida = fopen(arch, "a+");
	for (i=0; i<numProcesos ;i++){
		if(PIDS[i] != -2){
            char PID[8];
            sprintf(PID,"%ld",(long)PIDS[i]);
			entrada = fopen(PID,"r");
			caracter = fgetc(entrada);
			while (caracter != EOF){
				fprintf(salida, "%c",caracter);
				caracter = fgetc(entrada);
			}
			fclose(entrada);
		}
	} 
	fclose(salida);
}

///////////////////////////////////////////// MAIN /////////////////////////////////////////////
int main(int argc,char *argv[]){
	int numProcesos = atoi(argv[1]);
	int lineasArchivo = numeroDeLineas(argv[2]); //Lineas del archivo
	int lineasProceso = tareasPorProceso(lineasArchivo,numProcesos);
	//se crean los hijos y se hace map
	distribuirMap(argv[2],lineasArchivo,numProcesos,lineasProceso);
	//recibe los PIDs de los hijos y losalmacena
	pid_t *PIDS = malloc(sizeof(pid_t)*numProcesos); 
	recibirPIDS(numProcesos,PIDS);
	//se crea una estructura Par que contiene todos los pares generados por todos los hijos
	Par *pares = malloc(sizeof(Par));
    char PID[8];
	formarPares(pares,PIDS,numProcesos);
	free(pares);

	//reparte los pares entre los procesos
	int numPares = contarPares(pares);
	int tareasProceso = tareasPorProceso(numPares,numProcesos);
	//variables necesarias para el reduce
	int i,j;
	int turno = 0;
	pid_t childpid;
	// REDUCE //
	for(i=0; i<numProcesos;i++){
		if ((childpid = fork()) < 0){
			printf("No se ha podido crear el hijo\n");
		}
		else if(childpid == 0){
			Par *aux = pares;
			int sobra = numPares - numProcesos;
			for (j=0; j<numPares;j++){
				if (numProcesos >= numPares){
					if (i == j){
						char PID[8];
						sprintf(PID,"%ld",(long)getpid());
						reduce(aux,PID);
					}
				}
				else{
					if((i == j) || ((j>numProcesos-1) && (i == (j%numProcesos)))){
						char PID[8];
						sprintf(PID,"%ld",(long)getpid());
						reduce(aux,PID);
					}
				}
				aux = aux->sig;
			}
		}
	}
	//recibe los PIDs de los hijos y losalmacena
	PIDS = malloc(sizeof(pid_t)*numProcesos); 
	recibirPIDS(numProcesos,PIDS);
	//se escribe en el archivo indicado la salida del proceso
	escribirArchivoFinal(numProcesos,PIDS, argv[3]);
	//libera memoria
	free(PIDS);
	//liberaListaPares(pares);
	return 0;
}

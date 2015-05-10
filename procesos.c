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


typedef struct Persona{
	char *nombre;
	struct Persona *amigos;
} Persona;

typedef struct Par{
	char *amigo1;
	char *amigo2;
	struct Persona *amigos;
} Par;



void Map(int fileName, Persona *p, FILE *f,char *str1,char *str2,Persona *amigo);
// Los procesos no comparten datos por lo que hay que leer el archivo con cada fork y llegar a la linea que le corresponde
// buscar como es el peo con *argv[] y por que se le pasa argv[2] a fopen en vez de *argv[2]

int numeroDeLineas(FILE *f, char caracter){

	int lineas = 0;
		
	while ((caracter=fgetc(f)) != EOF){
		if (caracter == '\n'){
			lineas++;
		}
	}
	return lineas;
}


void escribirPar(FILE *archivo, char *persona1,char *persona2,Persona *p){//Pasa los dos nombres porque aveces no sera el primer amigo

	Persona *aux = p->amigos;
	fprintf(archivo,"%s %s",persona1,persona2); 
	while (aux != NULL){
		fprintf(archivo," %s",aux->nombre);
			aux = aux->amigos;
	}
	fprintf(archivo, "\n");
}


void eliminarLinea(int nombre,FILE *archivo,int numlinea,char *str1,char *str2,Persona *amigo, char *linea){
	//VA A HABER PROBLEMAS CON EL char caracter vs int caracter
	char caracter;
	int contador1,contador2,largo = 0;
	int MAX_buffer = 128;
	char *buffer = malloc(sizeof(char)*MAX_buffer);
	while(contador1 != EOF){
		if (contador1 == MAX_buffer){

			MAX_buffer *=2;
			contador1 = 0;
			buffer = realloc(buffer,MAX_buffer);
		}

		caracter = getc(archivo);
		if (contador2 != numlinea){
			buffer[largo] = caracter;
		}
		if (caracter == '\n'){
			contador2++;
		}
		largo++;
		contador1++;
	}
	fclose(archivo);
	char pid = (char)nombre;
	char *pid2 = &pid;
	archivo = fopen(pid2,"w");
	fprintf(archivo, "%s\n%s %s %s %c",buffer,str1,str2,linea,',');
	Persona *aux = amigo->amigos;
	while(aux != NULL){

		fprintf(archivo," %s",aux->nombre);
		aux = aux->amigos;

	}
	fprintf(archivo," %c",'\0');
	fclose(archivo);
}

//Guarda la parte restante de la linea (del segundo amigo en adelante)
char *guardarLinea(FILE *f){
	char caracter;
	int contador,largo = 0;
	int MAX_buffer = 128;
	char *linea = malloc(sizeof(char)*MAX_buffer);
	while((caracter != '\n') && (contador != EOF)){
		if (contador == MAX_buffer){

			MAX_buffer *=2;
			contador = 0;
			linea = realloc(linea,MAX_buffer);
		}
		caracter = getc(f);
		largo++;
		contador++;

	}
	return linea;
}

//retorna booleano true si lo encontro 
int buscarPar(int fileName, FILE *archivo,char *persona1,char *persona2,Persona *amigo){

	int encontrado = 0;
	char *linea;
	int contador = 0;
	char caracter;
	while(caracter != EOF){
		char *str1 = malloc(sizeof(char)*30);
		fscanf(archivo,"%s",str1);
		if(str1 == persona2){
			char *str2 = malloc(sizeof(char)*30);
			fscanf(archivo,"%s",str2);
			if (str2 == persona1){
				encontrado = 1;
				linea = guardarLinea(archivo);
				eliminarLinea(fileName,archivo,contador,str1,str2,amigo,linea);
				caracter = EOF;
			}
			else{
				while(caracter != '\n'){

					fscanf(archivo,"%s",str1);
					caracter = fgetc(archivo);
				}
				contador++;
			}	
		}
		else{
			while(caracter != '\n'){
				fscanf(archivo,"%s",str1);
				caracter = fgetc(archivo);
			}
			contador++;
		}
	}
	return encontrado;
}

struct Persona *leerLinea(FILE *archivo){

	char *str = malloc(sizeof(char)*30);
    fscanf(archivo, "%s" ,str);
    struct Persona *p = malloc(sizeof(Persona));
    p->nombre = str; //le asigna el nombre a la persona
    printf("%s -> ",p->nombre); //PRUEBA

    char caracter = 1;
    struct Persona *aux = p;

    //lee flecha
    char flecha[3];
    fscanf(archivo, "%s" ,flecha);

    while (caracter != '\n'){

       	char *strn = malloc(sizeof(char)*30);
	    fscanf(archivo, "%s" ,strn);

       	struct Persona *person = malloc(sizeof(Persona));
		person->nombre = strn;
		printf("%s, ",person->nombre);
		aux->amigos = person;
		aux = person;
		caracter = fgetc(archivo);
		if ((caracter == '\n')||(caracter == '\0')){
			aux->amigos = NULL;
		}
    }
    return p;
}

int main(int argc,char *argv[]){

	FILE *f = fopen(argv[2],"r");
	char caracter = fgetc(f);
	int lineas = numeroDeLineas(f, caracter);
	rewind(f);	
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
		if ((childpid = fork()) < 0){
			perror("fork:");
			exit(1);
		}
		else if(childpid == 0){
			rewind(f);			
			//printf("i: %d\t lineas_proceso: %d\n\n\n",i,lineas_proceso);
			int contador = 0;
			while ((caracter = fgetc(f)) != EOF){	
				if ((i*lineas_proceso <= contador) && (contador < (i+1)*lineas_proceso)){
					Persona *p = leerLinea(f);
					if (p->amigos->nombre != "-None-"){
						int pid_proceso = getpid();
						Map(pid_proceso,p,f,p->nombre,p->amigos->nombre,p->amigos);
					}
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

void Map(int fileName,Persona *p, FILE *f,char *str1,char *str2,Persona *amigo){

	if (!buscarPar(fileName,f,str1,str2,amigo)){
		fprintf(f,"%s %s",str1,str2);
		Persona *aux = amigo->amigos;
		while(aux != NULL){
			fprintf(f," %s",aux->nombre);
			aux = aux->amigos;
		}
		fprintf(f,"%c",'\0');

	}

}
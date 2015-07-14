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

//////////////////////////// Liberar espacio de estructura Par ////////////////////////////
void liberarPar(Par *p){
	free(p->amigo1);
	free(p->amigo2);
	liberarPersona(p->amigos1);
	if(p->amigos2!=NULL){
		liberarPersona(p->amigos2);
	}
    free(p->sig);
    free(p);
    printf("LIBERA PAR PERFECT\n");
}

///////////////////////// Libera la memoria del arreglo de Pares /////////////////////////
void liberaListaPares(Par *lista){
    Par *aux = lista;
    Par *aux2 = lista;
    while(lista!=NULL){
        printf("ENTRA CICLO\n");
        while(aux2!=NULL){
            printf("-----------ENTRA CICLO SIG\n");
            aux = aux2;
            aux2 = aux2->sig;
        }
        liberarPar(aux);
        aux = lista;
    }
    liberarPar(lista);
}

///////////////////////// lee el archivo y forma un string /////////////////////////
char *leerString(FILE *archivo,int *caracter){
    char *string;
    int caracter2 = 0;
    char *nombre = (char*)malloc(sizeof(char)*30);
    int i = 0; //esAmigo: booleano que indica si el nombre leido es el nombre de la persona o es un amigo de la lista
    int j = 0;
    int aumento = 30;
    while (1){
        *caracter = fgetc(archivo);
        if ((*caracter != 32) && (*caracter != EOF) && (*caracter != '\n')){ //Si es distinto del espacio va formando el string
            if (i < 30){
                nombre[i] = *caracter;
            }
            else{
                nombre = realloc(nombre,30+aumento);
                aumento+=30;
                if (nombre != NULL){
                    nombre[i] = *caracter;
                }
                else{
                    printf("Error de asignacion de memoria\n");
                    exit(-2); //Si el hijo termina mal el padre debe saber que no va a buscar en su archivo
                }
            }
            i++;
        }
        else{
            break;
        }
    }
    char *nombre2 = malloc(sizeof(char)*i+1); //filtra el string creado y lo coloca del tamano justo
    memcpy(nombre2,nombre,i);
    nombre2[i] = '\0';
    string = malloc(sizeof(char)*i+1);
    strcpy(string,nombre2);
    return string;  
}

//////////////////////////////////// Lee cada par del archivo ////////////////////////////////////
void leerPar(Par *pares,FILE *archivo,int *caracter){ 

    int encontrado = 0;
    char *nomb1,*nomb2,*flecha;
    nomb1 = leerString(archivo,caracter);
    nomb2 = leerString(archivo,caracter);
    flecha = leerString(archivo,caracter); //descarta la flecha
    if (*caracter != EOF){
        Persona *aux;
        Par *par = pares;
        if (pares->amigo1 == NULL){     //Si la lista de pares ta vacia. Asumiendo que se hace malloc de pares antes de llamar a leerPares
            pares->amigo1 = nomb1;
            pares->amigo2 = nomb2;
            pares->amigos1 = malloc(sizeof(Persona));
            aux = pares->amigos1;
            while ((*caracter != '\n') && (*caracter != EOF)){
                aux->nombre = leerString(archivo,caracter); //No usa malloc porque la funcion leerString lo hace
                if ((*caracter != '\n') && (*caracter != EOF)){
                    aux->amigos = malloc(sizeof(Persona));
                    aux = aux->amigos;
                }
            }
        }
        else{
            while(par != NULL){ //con que alguno de los dos dea distinto ya significa que es otro par
                if (strcmp(par->amigo1,nomb1) == 0 && strcmp(par->amigo2,nomb2) == 0){
                    encontrado = 1;                          //revisa hasta el final pero se queda parado en el ultimo de la lista
                    break;                                           //de pares para no perder la referencia a esta lista
                }
                if(par->sig != NULL){
                    par = par->sig;
                }
                else{
                    break;
                }
            }//Puede pasar que el ciclo anterior termine en el ultimo par porque son iguales o porque es el ultimo
            if(!encontrado){
                par->sig = malloc(sizeof(Par));
                par->sig->amigo1 = nomb1;
                par->sig->amigo2 = nomb2;
                par->sig->amigos1 = malloc(sizeof(Persona));
                aux = par->sig->amigos1;
                while ((*caracter != '\n') && (*caracter != EOF)){
                    aux->nombre = leerString(archivo,caracter); //No usa malloc porque la funcion leerString lo hace
                    if ((*caracter != '\n') && (*caracter != EOF)){
                        aux->amigos = malloc(sizeof(Persona));
                        aux = aux->amigos;
                    }
                }
            }
            else{ 
                par->amigos2 = malloc(sizeof(Persona));
                aux = par->amigos2;
                while ((*caracter != '\n') && (*caracter != EOF)){
                    aux->nombre = leerString(archivo,caracter); //No usa malloc porque la funcion leerString lo hace
                    if ((*caracter != '\n') && (*caracter != EOF)){
                        aux->amigos = malloc(sizeof(Persona));
                        aux = aux->amigos;
                    }
                }
            }
        }
    }
    free(flecha);
}

////////////////////////////////// cuenta los pares de la lista //////////////////////////////////
int contarPares(Par *pares){

    int contador = 0;
    Par *aux = pares;
    while(aux != NULL){
        if (aux->amigo1 != NULL){
            contador++;
        }
        aux = aux->sig;
    }
    return contador;

}

////////////////////////////imprime los pares ////////////////////////////
void imprimirPares(Par *pares){

    Par *aux = pares;
    while (aux != NULL){
        printf("%s %s\n",aux->amigo1,aux->amigo2);
        imprimirPersona(aux->amigos1);
        if (aux->amigos2 != NULL){
            imprimirPersona(aux->amigos2);
        }
        aux = aux->sig;
    }
}

///////////////////////forma la estructura de pares///////////////////////
void formarPares(Par *pares,pid_t *PIDS,int numProcesos){

    int i;
    for (i = 0;i<numProcesos; i++){
        if(PIDS[i] != -2){
            char PID[8];
            sprintf(PID,"%ld",(long)PIDS[i]);
            FILE *f = fopen(PID,"r");
            int caracter = 0;
            while(caracter != EOF){
                leerPar(pares,f,&caracter);
            }
            fclose(f);
        }
    }
}


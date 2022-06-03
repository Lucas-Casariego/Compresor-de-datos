#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "IO/io.h"
#include "BTree/btree.h"
#include "BTList/btlist.h"
#include "compresion.h"

void codificacionAuxArbol(BTree arbol, char** codificacion, struct ArrayWrapper arrWrapper, char bit){
  
  // Si es un nodo intermedio
  if (arbol->caracter == -1){
    // Agrego un solo char
    arrWrapper.buf[strlen(arrWrapper.buf)] = bit;
    arrWrapper.buf[strlen(arrWrapper.buf) + 1] = '\0';
    // Recursion
    codificacionAuxArbol(arbol->left, codificacion, arrWrapper, '0');
    codificacionAuxArbol(arbol->right, codificacion, arrWrapper, '1');
  }
  // Si es una hoja
  else{
    // Agrego un solo char
    arrWrapper.buf[strlen(arrWrapper.buf)] = bit;
    arrWrapper.buf[strlen(arrWrapper.buf) + 1] = '\0';
    codificacion[arbol->caracter] = malloc(sizeof(char) * strlen(arrWrapper.buf) + 1);
    strcpy(codificacion[arbol->caracter], arrWrapper.buf);
    // Si ya terminó el camino hasta la hoja, le asigno su codificacion (buf)
  }
}

void codificacionArbol(BTree arbol, char** codificacion){
  struct ArrayWrapper arrWrapper;
  arrWrapper.buf[0] = '\0';  // lo inicializamos
  codificacionAuxArbol(arbol->left, codificacion, arrWrapper, '0');
  codificacionAuxArbol(arbol->right, codificacion, arrWrapper, '1');
}


//Obtiene la lista de las hojas ordenadas por peso, y retorna un unico arbol
BTree arbolHuffman(BTList lista){

  while ( lista->sig->sig != NULL){
    //Obtiene los dos primeros arboles de la lista
    BTree nodo1 = lista->arbol;
    BTree nodo2 = lista->sig->arbol;

    lista = lista->sig->sig;
    free(lista->ant->ant);
    free(lista->ant);
    lista->ant = NULL;
    //Los une como hijos de un nuevo nodo
    BTree nuevoNodo = btreeUnir(nodo1, nodo2);
    //Agrega el nuevo nodo cread en la lista (donde corresponda segun su peso)
    lista = btlistAgregar(lista, nuevoNodo);
  }

  //Caso especial para cuando en la lista solo quedan dos items
  BTree arbolFinal = btreeUnir(lista->arbol, lista->sig->arbol);
  free(lista->sig);
  free(lista);

  return arbolFinal;
}


void serializarFormaAux( BTree arbol , char *buf){
  if(arbol->caracter == -1){
    strcat(buf, "0");
    serializarFormaAux(arbol->left, buf);
    serializarFormaAux(arbol->right, buf);
  } else {
    strcat(buf, "1");
  }
}

//Retorna la serializacion binaria de la forma del arbol
char *serializarForma( BTree arbol ){
  char *buf = malloc(100000*sizeof(char));
  strcpy(buf, "0");
  serializarFormaAux(arbol->left, buf);
  serializarFormaAux(arbol->right, buf);

  return buf;
}

void serializarHojasAux( BTree arbol, char *buf ){

  if(arbol->left != NULL)
    serializarHojasAux(arbol->left, buf);

  if(arbol->caracter != -1){
    char *hex = malloc(sizeof(char)*2);
    sprintf( hex, "%c", arbol->caracter);
    strcat(buf, hex);
    free(hex);
  }

  if(arbol->right != NULL)
    serializarHojasAux(arbol->right, buf);

}

//Retorna la serializacion de los caracteres del arbol recorriendo el arbol Inorder
char *serializarHojas( BTree arbol ){

  char *buf = malloc(sizeof(char)*256);
  strcpy(buf, "");
  serializarHojasAux(arbol->left, buf);
  serializarHojasAux(arbol->right, buf);
  return buf;
}

char *serializar( BTree arbol ){
  //Creo un int puntero en donde se almacena el largo de la serializacion de la forma.
  int *auxLen = malloc(sizeof(int));

  //Obtengo el binario de la serializacion de la forma
  char *serializacionFormaRaw = serializarForma(arbol);

  //Lo transformo en chars
  char *serializacionForma = implode(serializacionFormaRaw, strlen(serializacionFormaRaw), auxLen);

  //Obtengo la serializacion de las hojas (ya en chars)
  char *serializacionHojas = serializarHojas(arbol);

  //Creo un char puntero donde se almacena la serializacion
  char *buf = malloc(sizeof(char)*512);
  strcpy(buf, "");

  //Guardo ambas partes en el buf a retornar
  strcat(buf, serializacionForma);
  strcat(buf, serializacionHojas);
  //Limpieza
  free(auxLen);
  free(serializacionFormaRaw);
  free(serializacionForma);
  free(serializacionHojas);
  return buf;
}


// Genera el string que se guardará en el archivo .hf
char* comprimirInput(char** codificacion, char* input, int *lenInput){
  //Creo un int puntero en donde almacenar el largo de la serializacion de la forma.
  int *auxLen = malloc(sizeof(int));

  //Inicializo vacío un char puntero donde almaceno el binario de la compresion
  char *compresionRaw = malloc(sizeof(char)*256*(*lenInput));
  strcpy(compresionRaw, "");
  //Consumo caracter a caracter del archivo de entrada y lo codifico
  for (int i = 0; i < *lenInput; i++){
    strcat(compresionRaw, codificacion[(unsigned char)input[i]]);
  }
  //Transformo el binario a chars
  char *compresion = malloc(sizeof(char)*128*(*lenInput));
  compresion = implode(compresionRaw, strlen(compresionRaw), auxLen);
  //Limpieza
  free(auxLen);
  free(compresionRaw);
  return compresion;
}


// Genera los 2 archivos que almacenarán la compresión del texto
// y la serialización de la forma y las hojas del arbol
void compresion(char *path){
  int *lenInput = malloc(sizeof(int));
  char *buf = readfile(path, lenInput);
  //Inicializa arreglo con frecuencias
  int frecuencias[256];
  for(int index = 0; index < 256; index++){
    frecuencias[index] = 0;
  }

  //Calcula las frecuencias de cada caracter
  for(int pos = 0; pos < (*lenInput)-1; pos++){
    frecuencias[(int)buf[pos]]++;
  }

  //Genero un BTree para cada caracter, y lo agrego ordenenado segun su peso a una BTList
  BTList listaNodos = NULL;
  for(int index = 1; index < 256; index++){
    BTree charTree = btreeCrear(index, frecuencias[index]);
    listaNodos = btlistAgregar(listaNodos, charTree);
  }

  BTree arbol = arbolHuffman(listaNodos);

  //Arreglo con los pares [caracter : codificación]
  char** codificacion = malloc(sizeof(char*) * 256);
  codificacionArbol(arbol, codificacion);

  //Serializacion
  char *serializacion = serializar(arbol);
  //Armo el nombre del archivo donde almacenaré la serialización
  char *serializacionPath = malloc(sizeof(char)*100);
  strcpy(serializacionPath, "");
  strcat(serializacionPath, path);
  strcat(serializacionPath, ".tree");
  //Escribo el archivo

  writefile(serializacionPath, serializacion, strlen(serializacion));

  //Comprimo el texto con la codificación y lo almaceno en compresión (como chars)
  char *compresion = comprimirInput(codificacion, buf, lenInput);
  //Armo el nombre del archivo donde almacenare la compresion
  char *compresionPath = malloc(sizeof(char)*100);
  strcpy(compresionPath, "");
  strcat(compresionPath, path);
  strcat(compresionPath, ".hf");
  //Escribo el archivo
  writefile(compresionPath, compresion, strlen(compresion));

  printf("Generados los archivos %s y %s\n", compresionPath, serializacionPath);
  //Limpieza
  for(int index = 0; index < 256; index++){
    free(codificacion[index]);
  }
  free(codificacion);
  free(lenInput);
  free(buf);
  free(compresion);
  free(compresionPath);
  free(serializacion);
  free(serializacionPath);
  btreeDestruir(arbol);
}

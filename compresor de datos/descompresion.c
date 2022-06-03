#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "IO/io.h"
#include "BTree/btree.h"
#include "BTList/btlist.h"

//   ============================  descompresión  ====================================

char *generar_output( BTree arbol, char *compresion, int lenInput){

  int *auxLen = malloc(sizeof(int));
  //Transformo la compresion a su forma binaria
  char *compresionRaw = explode( compresion, lenInput, auxLen);
  char *output = malloc(sizeof(char)*256*lenInput);
  strcpy(output, "");
  int outputIndex = 0;

  BTree nodoIndex = arbol;            //Arbol auxiliar para recorrer
  int index = 0;
  
  while(index < *auxLen){

    if(nodoIndex->caracter == -1){  //Si estoy parado en un nodo intermedio
      //Consumo del binario, si hay un 0 continuo con el hijo izquierdo. Sino, con el derecho.
      if( compresionRaw[index] == '0' ){
        nodoIndex = nodoIndex->left;
      } else {
        nodoIndex = nodoIndex->right;
      }
      index++;
    } else {
      //Si estoy parado en una hoja, agrego el caracter al archivo de salida
      output[outputIndex] = nodoIndex->caracter;
      outputIndex++;
      nodoIndex = arbol;
    }
  }

  //Limpieza
  free(auxLen);
  free(compresionRaw);

  return output;
}

BTree generarArbolForma( char *serializacionForma, int *index ){

  BTree arbolForma = btreeCrear(-1,-1);

  if( serializacionForma[*index] == '0'){
    (*index)++;
    arbolForma->left = generarArbolForma(serializacionForma, index);
    (*index)++;
    arbolForma->right = generarArbolForma(serializacionForma, index);
  }

  return arbolForma;
}

// Rearma la forma del arbol desde su serialización (char* de bits)
BTree parsear_arbol(char* serializacion_forma_bin){

  int* cont = malloc(sizeof(int));
  *cont = 0;

  BTree arbolForma = generarArbolForma( serializacion_forma_bin, cont);

  free(cont);
  return arbolForma;
}

// ======================  hojas  ==============================

void parsear_hojas_rec(BTree arbol_forma, char* serializacion_hojas, int* cont){
  // es nodo
  if (arbol_forma->left != NULL && arbol_forma->right != NULL){
    parsear_hojas_rec(arbol_forma->left, serializacion_hojas, cont);
    parsear_hojas_rec(arbol_forma->right, serializacion_hojas, cont);
  }
  // es hoja
  else if (arbol_forma->left == NULL && arbol_forma->right == NULL){
    arbol_forma->caracter = (int)serializacion_hojas[*cont];     // almacenamos el char casteado a int
    *cont += 1;
  }
}

BTree parsear_hojas(BTree arbol, char* serializacion_hojas_chars){
  int* cont = malloc(sizeof(int));
  *cont = 0;
  parsear_hojas_rec(arbol, serializacion_hojas_chars, cont);

  //Limpieza
  free(cont);

  return arbol;   // arbol rearmado desde la serialización
}



void decompresion(char *path){
  int *lenCompresion = malloc(sizeof(int));
  int *lenSerializacion = malloc(sizeof(int));
  int *lenSerializacionRaw = malloc(sizeof(int));

  char *serializacionPath = malloc(sizeof(char)*100);
  strcpy(serializacionPath, "");
  strcat(serializacionPath, path);
  strcat(serializacionPath, ".tree");   // Nombre del archivo con la serializacion

  char* serializacion = readfile(serializacionPath, lenSerializacion);

  char* serializacionForma = malloc(sizeof(char) * 64);
  char* serializacionHojas = malloc(sizeof(char) * 255);

  for (int i = 0; i < 64; i++){
      serializacionForma[i] = serializacion[i];
  }

  char* serializacionFormaRaw = explode(serializacionForma, 64, lenSerializacionRaw);

  serializacionFormaRaw[0] = serializacionFormaRaw[0];

  for (int i = 0; i < 255; i++){
      serializacionHojas[i] = serializacion[i+64];
  }

  BTree arbolForma = parsear_arbol(serializacionFormaRaw);
  BTree arbolFinal = parsear_hojas(arbolForma, serializacionHojas);

  //Descompresion del .hf
  char *compresionPath = malloc(sizeof(char)*100);
  strcpy(compresionPath, "");
  strcat(compresionPath, path);
  strcat(compresionPath, ".hf");   // Nombre del archivo con la serializacion

  char *compresion = readfile(compresionPath, lenCompresion);

  char *output = generar_output(arbolFinal, compresion, *lenCompresion);

  char *outputPath = malloc(sizeof(char)*100);
  strcpy(outputPath, "");
  strcat(outputPath, path);
  strcat(outputPath, ".dec");   // Nombre del archivo con la serializacion

  writefile(outputPath, output, strlen(output));

  printf("Generado el archivo %s\n", outputPath);

  //Limpieza
  free(lenCompresion);
  free(compresionPath);
  free(compresion);

  free(lenSerializacionRaw);
  free(lenSerializacion);
  free(serializacion);
  free(serializacionPath);
  free(serializacionHojas);
  free(serializacionForma);
  free(serializacionFormaRaw);
  free(output);
  free(outputPath);
  btreeDestruir(arbolFinal);
}

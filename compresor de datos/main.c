#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "IO/io.h"
#include "BTree/btree.h"
#include "BTList/btlist.h"
#include "compresion.h"
#include "descompresion.h"

int main(int argc, char* argv[]){  // char* argv[]

  if(argc != 3){
    printf("Error en la cantidad de argumentos\n");
    return 0;
  }

  // Accion: 'C' para comprimir, 'D' para descomprimir
  if(strcmp(argv[1],"C") == 0){
    compresion(argv[2]);
  } else if(strcmp(argv[1],"D") == 0){
    decompresion(argv[2]);
  } else {
    printf("Error en la accion a realizar\n");
  }

  return 0;
}

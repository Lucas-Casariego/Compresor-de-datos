#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "btree.h"

BTree btreeCrear( int caracter, int peso ){
  BTree nuevoTree = malloc(sizeof(BTNodo));
  nuevoTree->caracter = caracter;
  nuevoTree->peso = peso;
  nuevoTree->left = NULL;
  nuevoTree->right = NULL;
  return nuevoTree;
}

void btreeDestruir(BTree nodo) {
  if (nodo != NULL) {
    btreeDestruir(nodo->left);
    btreeDestruir(nodo->right);
    free(nodo);
  }
}

BTree btreeUnir(BTree left, BTree right) {
  BTree nuevoNodo = malloc(sizeof(BTNodo));
  assert(nuevoNodo != NULL);
  nuevoNodo->caracter = -1;
  nuevoNodo->left = left;
  nuevoNodo->right = right;
  nuevoNodo->peso = left->peso + right->peso;
  return nuevoNodo;
}  

void btreeImprimir(BTree arbol){
  if(arbol->left){
    //printf("Left\n");
    btreeImprimir(arbol->left);
  } else{
    printf("Char: %d\n", arbol->caracter);
  }
  if(arbol->right){
    //printf("Right\n");
    btreeImprimir(arbol->right);
  }

}

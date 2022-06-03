#ifndef __BTLIST_H__
#define __BTLIST_H__

#include "../BTree/btree.h"

//Lista doblemente enlazada de Binary Trees

typedef struct _BTNode {
  BTree arbol;
  struct _BTNode *ant;
  struct _BTNode *sig;
} BTNode;

typedef BTNode *BTList;

/**
 * Recorre la lista imprimiendo caracter y peso
 */
void btlistImprimir(BTList lista);

/**
 * Agrega un elemento al inicio
 */
BTList btlistAgregar(BTList lista, BTree arbol);

#endif /* __BTLIST_H__ */

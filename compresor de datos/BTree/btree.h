#ifndef __BTREE_H__
#define __BTREE_H__

typedef void (*FuncionVisitante)(int dato);

typedef struct _BTNodo {
  int caracter;
  int peso;
  struct _BTNodo *left;
  struct _BTNodo *right;
} BTNodo;

typedef BTNodo *BTree;

/**
 * Devuelve un arbol con el peso y el ASCII pasado por argumento.
 */
BTree btreeCrear( int caracter, int peso );

/**
 * Destruye del árbol.
 */
void btreeDestruir(BTree nodo);

/**
 * Crea un nuevo arbol con los argumentos pasados como hijos
 */
BTree btreeUnir(BTree left, BTree right);

/**
 * Imprime un arbol
 */
void btreeImprimir(BTree arbol);

#endif /* __BTREE_H__ */

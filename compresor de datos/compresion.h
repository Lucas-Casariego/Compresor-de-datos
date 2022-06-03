#ifndef __COMPRESION_H__
#define __COMPRESION_H__


struct ArrayWrapper {
  char buf[255];
};


/**
 * funcion aux recursiva, se encarga de almacenar en char** codificacion, la codificacion binaria de cada caracter ubicado en las hojas del arbol
 * para esto voy recorriendo el arbol y contcatenando '0' o '1' (izquierda o derecha respectivamente) en un buf y cuando llega a la hoja (caracter)
 * guarda en char** codificacion, la codificación obtenida en buf hasta el momento.
 * para que el array buf no se pase por referencia, a este lo ubico dentro de una estructura.
 */

void codificacionAuxArbol(BTree arbol, char** codificacion, struct ArrayWrapper arrWrapper, char bit);

void codificacionArbol(BTree arbol, char** codificacion);

/**
 * Obtiene la lista de las hojas ordenadas por peso, y retorna un unico arbol
 */
BTree arbolHuffman(BTList lista);


void serializarFormaAux( BTree arbol , char *buf);
/**
 * Retorna la serializacion binaria de la forma del arbol
 */
char *serializarForma( BTree arbol );


void serializarHojasAux( BTree arbol, char *buf );
/**
 * Retorna la serializacion de los caracteres del arbol recorriendo el arbol Inorder
 */
char *serializarHojas( BTree arbol );

/**
 * con retorna un char* buf con la serialización del arbol
 */
char *serializar( BTree arbol );



/**
 *  Genera el string que se guardará en el archivo .hf
 */
char* comprimirInput(char** codificacion, char* input, int *lenInput);



/**
 * Genera los 2 archivos que almacenarán la compresión del texto
 * 
 */
void compresion(char *path);


#endif

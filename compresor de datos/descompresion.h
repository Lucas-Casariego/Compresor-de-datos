#ifndef __DESCOMPRESION_H__
#define __DESCOMPRESION_H__

/**
 * toma el arbol, el .hf entonces decodifica y genera el archivo de salida 
 */
char *generar_output( BTree arbol, char *compresion, int lenInput);


BTree generarArbolForma( char *serializacionForma, int *index );
/**
 * Rearma la forma del arbol desde su serialización (char* de bits)
 */
BTree parsear_arbol(char* serializacion_forma_bin);


void parsear_hojas_rec(BTree arbol_forma, char* serializacion_hojas, int* cont);
/**
 * con la estructura del arbol y la serialización de las hojas (char*) retorna el arbol rearmado 
 */
BTree parsear_hojas(BTree arbol_forma, char* serializacion_hojas_chars);


/**
 * genera el archivo decodificado a partir de los archivos .hf y .tree
 */
void decompresion(char *path);


#endif

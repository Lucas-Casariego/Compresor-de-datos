Este es un trabajo práctico grupal (2 personas) que hicimos en la facultad.
Este es un compresor de datos hecho en lenguaje C. 
El mismo va a funcionar para cualquier tipo de archivo y es sin pérdida, es decir, 
desde el archivo comprimido podemos reconstruir exactamente el original 
(a diferencia de la compresión con pérdida, por ejemplo el formato JPG para imágenes, que puede perder algunos detalles).


El compresor se basa en la codificación de Huffman. La idea de la misma es analizar la frecuencia con
la que aparecen los caracteres en el texto de entrada, y optimizar a aquellos que son más frecuentes.
Mediante un algoritmo (que detallo adelante) se le asigna una secuencia de bits a cada caracter, con
las siguientes propiedades:
- Los caracteres que aparecen más frecuentemente tienen secuencias más cortas. Esto es lo que nos
hace ahorrar espacio.
- Ninguna secuencia es prefijo de otra, es decir, tenemos lo que se llama un código de prefijos. Esto
es necesario para que, desde el archivo comprimido, podamos identificar exactamente los caracteres
representados en el mismo.

El primer problema consiste en cómo conseguir una buena codificación de prefijos.
David Huffman encontró en 1952 un algoritmo que garantiza conseguir la codificación óptima. 
El algoritmo consiste en:

- Computar las frecuencias de cada símbolo. Es decir, cuántas veces aparece cada símbolo en la
entrada.

- Crear, para cada símbolo s de frecuencia w, un árbol binario consistiendo de una unica hoja s. Se
le asigna a este arbol el “peso” w.

- Mientras haya más de un árbol, tomamos dos de menor peso y los unimos en uno solo mediante un
nuevo nodo. El peso del nuevo árbol es la suma de los dos pesos originales.



1) Compresión

Para comprimir, tenemos que solamente mapear cada símbolo a su secuencia de bits. Esto puede hacerse
guardando un arreglo que indique, para cada símbolo su codificación. Este arreglo puede crearse desde el
árbol resultante del Algoritmo de Huffman.

2) Descompresión

Para descomprimir, partimos desde una secuencia de bits, y tenemos que encontrar los símbolos que la
generan. Iterativamente, tenemos que traducir algún prefijo (de longitud desconocida) de la secuencia a
un símbolo. Para hacer esto, nos paramos en la raíz del árbol y seguimos el siguiente procedimiento:

- Si el primer bit es un cero, nos movemos hacia el hijo izquierdo. Si no, nos movemos hacia el hijo
derecho.

- Avanzamos en la cadena (es decir “consumimos” el bit).

- Si llegamos a una hoja del árbol, ese es el símbolo que buscamos.

- Si no llegamos a una hoja, repetimos

Al terminar ese procedimiento, habremos decodificado un símbolo de la cadena. Repetimos esto hasta
haber consumido toda la cadena de bits y terminamos.

3) Serialización

Nos falta un ingrediente para implementar el compresor/descompresor: tenemos que comunicar de alguna
manera cuál es la codificación. Si el compresor escribe solamente un archivo con la codificación símbolo-a-símbolo,
el descompresor no tiene manera de saber cuales eran los símbolos originales.
Necesitamos entonces una forma de guardar el árbol en un archivo para que pueda ser leído por el
descompresor

3.1) Serializando la forma

Hay muchas formas de lograr esto, para distintos tipos de árboles. En nuestro caso, tenemos árboles algo
particulares, ya que:
- No tenemos valores en los nodos, sólo los tenemos en las hojas.
- Cada nodo tiene 0 hijos (las hojas), o 2 (el resto). Ningún nodo tiene 1 hijo. 
  Se dice que el árbol está lleno (full).
Esto nos permite tener una codificación relativamente simple. Primero, nos preocupamos por codificar
la forma del árbol, luego pensaremos en los valores que contienen las hojas. Seguiremos el siguiente
procedimiento para codificar la forma de un árbol T.
Si T es una hoja, imprimimos 1.
Si T es un nodo, imprimimos 0, luego la codificación de su hijo izquierdo, y luego la codificación de
su hijo derecho.

3.2) Serializando los valores de las hojas

Una vez que serializamos la forma del árbol, debemos representar los valores de cada hoja. Dado que
cada valor ocupa exactamente 1 byte (son caracteres) y que conocemos la cantidad de hojas (exactamente
256), podemos simplemente guardarlos (en orden) uno después del otro, inmediatamente después de la
serialización de la forma del árbol.

Para reconstruir el árbol, primeros creamos un árbol de la forma correcta sin ningún valor en sus hojas.
Luego, hacemos un recorrido en orden consumiendo caracteres del archivo y guardándolos en las hojas.
Este será el contenido del archivo .tree que usa el descompresor.


Aquí hay un video que explica el funcionamiento de la codificación de Huffman: https://www.youtube.com/watch?v=85NFcU8yTSs

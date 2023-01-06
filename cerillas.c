#define	TRUE	1
#define	FALSE	0
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*	tipo de datos "segmentos":
		vector de 7 shorts, almacena un dígito en forma de segmentos, de la siguiente forma:
			|-0-|
			1   2
			|-3-|
			4   5
			|-6-|
		por ejemplo el 0 se almacenaría como:
			segmentos numero0 = {1,1,1,0,1,1,1};

	conjuntos de segmentos:
		permiten almacenar un número completo compuesto de varios dígitos, por ejemplo para almacenar números de 4 dígitos:
			segmentos numero[4];
		para hacer lo mismo en tiempo de ejecución:
			segmentos *numero;
			numero = (segmentos *) calloc(4, sizeof(segmentos));
*/
typedef short segmentos[7];

//	especificación de los dígitos del 0 al 9 en forma de segmentos (vector de 10x7 shorts)
segmentos digitos[10] = {
	{1,1,1,0,1,1,1},
	{0,0,1,0,0,1,0},
	{1,0,1,1,1,0,1},
	{1,0,1,1,0,1,1},
	{0,1,1,1,0,1,0},
	{1,1,0,1,0,1,1},
	{1,1,0,1,1,1,1},
	{1,0,1,0,0,1,0},
	{1,1,1,1,1,1,1},
	{1,1,1,1,0,1,1}
};

//	copia los 7 segmentos de un dígito
void CopiarSegmentos(segmentos destino, segmentos origen)
{
	memcpy(destino, origen, 7 * sizeof(destino[0]));
}

//	transforma un entero "origen" a un vector de segmentos "destino" (no un solo dígito sino un número completo compuesto de "tam" dígitos)
void IntSegmento(segmentos *destino, int origen, int tam)
{
	while (tam >= 1) {
		tam--;
		CopiarSegmentos(destino[tam], digitos[origen % 10]);
		origen = origen / 10;
	}
}

/*	compara dos números de "tam" dígitos
	devuelve TRUE si entre los dos se han movido "ndiferencias" segmentos
*/
int CompararSegmentos(segmentos *origen, segmentos *destino, int tam, int ndiferencias)
{
	int i;
	int diferencias = 0, total = 0;

	for (i=0; i < tam*7; i++) {
		total += (*origen)[i] - (*destino)[i];
		if ((*origen)[i] != (*destino)[i])
			diferencias++;
	}

	// total es la diferencia en el total de segmentos, y debe ser 0 (mismo nº de cerillas en los dos casos)
	// diferencias es el total de segmentos cambiados (es el doble del nº de cerillas movidas)
	if (diferencias == ndiferencias*2 && total == 0)
		return TRUE;
	else
		return FALSE;
}

/*	parámetros del programa: el número inicial y el número de cerillas que hay que mover

	retorno:
		0	sin errores
		1	error en el número de parámetros pasados
		2	error en el formato de los parámetros
*/
int main(int argc, char *argv[])
{
	int inicial, ndiferencias, tam; segmentos *s_inicial;
	int candidato; segmentos *s_candidato;

	// gestión de parámetros de entrada
	if (argc != 3) {
		fprintf(stderr, "Uso: %s\nEjemplo: %s 5912 2\n", argv[0], argv[0]);
		return 1;
	}

	inicial = atoi(argv[1]);		// ej: 5912
	ndiferencias = atoi(argv[2]);		// ej: 2
	if (inicial == 0 || ndiferencias == 0) {
		fprintf(stderr, "Los parámetros tienen que ser numéricos y distintos de 0\nEjemplo: %s 5912 2\n", argv[0]);
		return 2;
	}

	tam = strlen(argv[1]);			// ej: 4
	candidato = (int) pow(10,tam) - 1;	// ej: 9999

	s_inicial = (segmentos *) calloc(tam, sizeof(segmentos));
	s_candidato = (segmentos *) calloc(tam, sizeof(segmentos));

	// pasar el nº inicial a segmentos y comparar con cada nº candidato decrecientemente
	IntSegmento(s_inicial, inicial, tam);
	while (candidato >= 0) {
		IntSegmento(s_candidato, candidato, tam);
		if (CompararSegmentos(s_inicial, s_candidato, tam, ndiferencias))
			printf("%d\n", candidato);
		candidato--;
	}

	return 0;
}

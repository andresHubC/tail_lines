#include <stdio.h>
#include <string.h>

#define MAXLINES 100		/* numero de lineas de texto que puede recibir el programa */
int readlines(char *lineptr[], int);
void writelines(char *lineptr[], int, int);

int main(void)
{
	int nlines;					/* numero de lineas de texto que han sido leidas */
	int n = 2;					/* valor que indica las ultimas n lineas que imprime el programa, por definicion n = 1 */
	char *lineptr[MAXLINES]; 	/* apuntadores a las lineas de texto ingresadas */
	if ((nlines = readlines(lineptr, MAXLINES)) >= 0 && nlines >= n) {
		writelines(lineptr, nlines, n);
		return 0;
	}
	else {
		if (nlines != -1)
			printf("ERROR: El total de lineas ingresadas (%d) es menor al numero de lineas que se desean imprimir (n = %d).", nlines, n);
		return 1;
	}
}

#define MAXLEN 400				/* longitud maxima de caracteres en una linea de texto de entrada */
#define MAXSPACE 10000			/* espacio del arreglo donde se localizan cada una de las lineas */
int getline(char s[], int);

/* readlines: lee las lineas de texto en la entrada */
int readlines(char *lineptr[], int maxlines)
{
	int len;							/* longitud de la linea de texto que este en line */
	int nlines;							/* total de lineas de texto leidas por el programa */
	char line[MAXLEN];					/* line guarda temporalmente la linea de texto que toma getline */
	static char linestore[MAXSPACE];	/* almacen de las lineas de texto ingresadas */
	char *storefp;						/* apuntador a la siguiente posicion libre en linestore */
	
	nlines = 0;
	storefp = linestore;
	while ((len = getline(line, MAXLEN)) > 0)
		if (nlines >= maxlines || (linestore + MAXSPACE - storefp) < len) {
			printf("ERROR: Espacio lleno. No es posible leer la ultima linea de texto ingresada.");
			return -1;
		}
		else { 
			strcpy(storefp, line);
			lineptr[nlines++] = storefp;
			storefp += len;
		}
	if (len == -1) {
		printf("ERROR: La linea de texto ingresada es muy extensa. No es posible leerla.");
		return -1;
	}
	return nlines;
}

/* writelines: imprime las ultimas n lineas de texto en la pantalla del usuario. */
void writelines(char *lineptr[], int nlines, int n)
{
	lineptr += nlines - n;	/* lineptr se posiciona n lineas atras respecto a la ultima linea */
	while (n-- > 0)
		printf("%s\n", *lineptr++);
}

/* getline: toma la linea de entrada y la guarda en s, devuelve la longitud de caracteres de la linea */
/*			incluyendo el caracter nulo. En caso de que la linea exceda el lim de espacio en el arreglo */
/*			se devuelve -1, indicando error al guardar la linea de texto */
int getline(char s[], int lim)
{
	int c, i;
	
	i = 0;
	while ((c = getchar()) == ' ' || c == '\t')
		;
	while (--lim > 0 && c != EOF && c != '\n') {
		s[i++] = c;
		c = getchar();
	}
	if (i > 0)
		s[i++] = '\0';
	if (lim == 0 && c != EOF && c != '\n')
		i = -1;
	return i;
}
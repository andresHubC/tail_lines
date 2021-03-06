#include <stdio.h>
#define MAXLINES 100		/* numero de lineas de texto que puede imprimir el programa */
#define PLINES 10			/* numero de lineas a imprimir en caso de no recibir un valor del usuario */

int readlines(char *lineptr[], int);
void writelines(char *lineptr[], int, int, int);
int getint(char *);

/* El programa acepta un segundo argumento en la linea de comandos, el argumento debe ser un numero entero positivo */
/* El primer digito del numero tiene que ser antecedido por un guion, como el siguiente ejemplo: -10  */
int main(int argc, char *argv[])
{
	int nlines;					/* numero de lineas de texto que han sido leidas */
	int n;						/* valor que indica las ultimas n lineas que imprime el programa */
	char *lineptr[MAXLINES]; 	/* apuntadores a las lineas de texto ingresadas */
	
	if (argc == 1)
		n = PLINES;			/* n toma un valor predefinido por el programa */
	else if (argc == 2) 
		if ((*++argv)[0] == '-') {
			n = getint(++argv[0]);
			if (n == -1)
				printf("ERROR: El programa no puede convertir el argumento ingresado a un numero entero.\n");
			else if (n == 0) {
				printf("ERROR: El argumento ingresado no es un numero entero positivo.\n");
				n = -1;
			}
			else if (n > MAXLINES) {
				printf("ERROR: El programa permite imprimir las ultimas %d lineas. El valor de n excede el limite.\n", MAXLINES);
				n = -1;
			}
		}
		else {
			n = -1;
			printf("ERROR: El argumento ingresado no es valido, revisa la forma de ingresarlo.\n");
		}
	else {
		printf("ERROR: Solo se permite el ingreso de dos argumentos en la linea de comando: tail -n\n");
		n = -1;
	}
	if (n == -1) 
		return 1;
	printf("El programa recibe un determinado numero de lineas de texto.\n"
		   "Despues de ingresar las lineas de texto se mostrara al usuario las ultimas %d lineas recibidas.\n"
		   "El programa identifica el final de texto de dos formas:\n"
		   "- Si la entrada de datos es un archivo, al leer EOF\n"
		   "- Si la entrada de datos es manual, al ingresar una linea en blanco (sin caracteres legibles)\n\n", n);
	printf("A continuacion ingresa las lineas de texto:\n");
	if ((nlines = readlines(lineptr, MAXLINES)) < 0)
		return 1;
	else {
		writelines(lineptr, nlines, n, MAXLINES);
		return 0;
	}
}

#include <string.h>
#define MAXLEN 150				/* longitud de una linea de texto ingresada, incluyendo el caracter nulo */
#define MAXSPACE 15000			/* espacio del arreglo donde se localizan cada una de las lineas */
int getline(char s[], int);

/* readlines: lee las lineas de texto en la entrada */
int readlines(char *lineptr[], int maxlines)
{
	int len;							/* longitud de la linea de texto en line */
	int nlines;							/* total de lineas de texto leidas por el programa */
	int index;							/* index establece el siguiente elemento en lineptr para guardar el apuntador a una linea */
	char line[MAXLEN];					/* line guarda temporalmente la linea de texto que toma getline */
	static char linestore[MAXSPACE];	/* arreglo donde se almacenan las lineas de texto ingresadas */
	char *storep;						/* apuntador a la siguiente posicion libre en linestore */
	
	nlines = index = 0;
	storep = linestore;
	while ((len = getline(line, MAXLEN)) > 0) {
		if ((linestore + MAXSPACE - storep) < len) 
			storep = linestore;			/* Se apunta al inicio de linestore, para reutilizar el espacio */
		strcpy(storep, line);
		lineptr[index] = storep;
		storep += len;
		if (++index == MAXLINES)
			index = 0;					/* vuelve a la posicion inicial cuando su valor es igual al limite de elementos en lineptr */
		nlines++;
	}
	if (len == -1) {
		printf("ERROR: La linea de texto ingresada es muy extensa. No es posible leerla.\n");
		nlines = -1;
	}
	return nlines;
}

/* writelines: imprime las ultimas n lineas de texto en la pantalla del usuario. */
void writelines(char *lineptr[], int nlines, int n, int maxlines)
{
	int i;
	
	if (nlines < n) 				/* El programa imprime todas las lineas ingresadas al no tener las n lineas deseadas */
		n = nlines;

	i = (nlines % maxlines) - n;	/* i guarda la posicion de la primera linea de texto a imprimir */	
	if (i < 0)						/* al ser negativo, se suma la cantidad de apuntadores a lineas de texto para obtener la posicion */
		i += maxlines;
	printf("\n\nLineas de texto:\n");
	if (nlines == 0)
		printf("No se ingresaron lineas de texto.\n");
	while (n-- > 0) {
		printf("%s\n", lineptr[i]);
		i = (i + 1) % maxlines;
	}
}

/* getline: toma la linea de entrada y la guarda en s, devuelve la longitud de caracteres de la linea */
/*			incluyendo el caracter nulo. En caso de que la linea exceda el lim de espacio en el arreglo */
/*			se devuelve -1, indicando error al guardar la linea de texto */
int getline(char s[], int lim)
{
	int c, i;			
	
	i = 0;		/* i indica la posicion disponible para almacenar los caracteres en el arreglo s[] y ademas contabiliza el total de caracteres. */
	while ((c = getchar()) == ' ' || c == '\t')		/* Se omiten los espacios en blanco al inicio */
		;
	while (--lim > 0 && c != EOF && c != '\n') {
		s[i++] = c;
		c = getchar();
	}
	if (i > 0)			/* Cuando se ingresa al menos un caracter, se le incorpora el caracter nulo */
		s[i++] = '\0';
	if (lim == 0 && c != EOF && c != '\n')		/* Cuando se ingresa una linea extensa se notifica al usuario que no es posible guardarla */
		i = -1;
	return i;		
}

#include <ctype.h>
#include <limits.h>
#define DIG 11    /* Maximo numero de digitos, incluyendo el caracter nulo, en la cadena numerica maxint */
void itoa(int, char maxint[]);

/* getint: Toma el numero entero positivo que se ingreso como un argumento en la linea de comandos */
int getint(char *s)
{
	int num, i, len;
	char maxint[DIG];	
	
	while (*s == ' ' || *s == '\t')	/* Se omiten los espacios en blanco al inicio */
		s++;
	if (*s == '+')	 /* Se omite el signo positivo al inicio del numero. */
		s++;
	itoa(INT_MAX, maxint);     /* El maximo valor de un int es convertido a cadena de caracteres */
	len = strlen(s);    
	
	/* El programa verifica que s sea un numero y no sobrepase el valor limite */
	if (len == 0 || len > strlen(maxint))	
		return -1;
	if (len == strlen(maxint))		
		for (i = 0; i < len; i++)
			if (!isdigit(s[i]) || s[i] > maxint[i])
				return -1;
	for (num = 0; isdigit(*s); s++)
		num = 10 * num + (*s - '0');
	if (*s != '\0')
		num = -1;	/* Se indica que no es un numero */
	return num;
}

/* itoa: Convierte el valor maximo que tiene un dato int en cadena de caracteres */
void itoa(int n, char maxint[])
{
	static int i;
	
	i = 0;
	if (n / 10)
		itoa(n / 10, maxint);
	maxint[i++] = n % 10 + '0';
	maxint[i] = '\0';
}
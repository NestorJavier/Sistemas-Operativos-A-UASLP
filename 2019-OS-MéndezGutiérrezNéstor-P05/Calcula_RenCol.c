#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> //libreria
#include <sys/types.h>//para el manejo de la estructura pid_t
#include <sys/wait.h>//libreria para la función wait

/****************************************************
*													*
*	Profesor:	Marcela Ortiz Hernández				*
*	Fecha:		19 de Septiembre de 2019			*
*													*
*	Alumnos:	Hernández Pérez Priscila Itzel		*
*				Méndez Gutiérrez Néstor Javier		*
*				Franco Tenorio Roberto Carlos		*
*													*
****************************************************/


//./Calcula_RenCol mat1.txt mat2.txt 1 3

//argc: Cantidad de argumentos en la linea de comandos
//argv: Valor de los argumentos, el primer argumento siempre es el nombre del programa
// si no hay argumentos la cuenta es 1

void cargaArchivo(FILE *FArch, int **m, int tam, int numMatriz);
void LiberaMemoria(int **m1, int **m2, int tam);
void ReservaMemoria(int **m1, int **m2, int tam);
void imprimeMatriz(int **m, int tam);

int main(int argc, char *argv[])
{
	int Ren, Col, i, iRes = 0;
	int tam, **m1, **m2;
	char tam1[4], tam2[4];

	if(argc == 5)
	{
		FILE * FArch1 = fopen(argv[3], "rb");
		FILE * FArch2 = fopen(argv[4], "rb");

		if(FArch1 == NULL || FArch2 == NULL)
		{
			printf("error al abrir el archivo");
			exit(1);
		}

		fgets(tam1, 4, FArch1);
		fgets(tam2, 4, FArch2);
		tam = atoi(tam1);

		m1 = (int**)malloc(tam*sizeof(int*));
		m2 = (int**)malloc(tam*sizeof(int*));	
		if(m1 == NULL || m2 == NULL)
		{
			printf("No se ha podido reservar espacio en memoria\n");
			exit(1);
		}

		ReservaMemoria(m1, m2, tam);
		cargaArchivo(FArch1, m1, tam, 1);
		cargaArchivo(FArch2, m2, tam, 2);


		//////////////////////////////////////////////////////////////////////
		int Ren = atoi(argv[1]);
		int Col = atoi(argv[2]);

		for(i = 0; i < tam; i++)
		{
			iRes = iRes + (m1[Ren][i]*m2[Col][i]);
		}
		printf("Proceso: %d Resultado: %d\n",getpid(), iRes);
		// printf("Resultado: %d\n", iRes);
		//Convierte el numero de cual se ha de calcular el facotrial a entero
	}	
	else
		printf("Error en los argumentos\n");	
	
	//Regresamos el resultado de el calculo al proceso padre
	exit(iRes);
}

void imprimeMatriz(int **m, int tam)
{
	int i, j;
	for(i = 0; i < tam; i++)
	{
		for(j = 0; j < tam; j++)
		{
			printf("%d ",m[i][j]);
		}
		printf("\n");
	}
}


void ReservaMemoria(int **m1, int **m2, int tam)
{
	int i;
	for(i = 0; i < tam; i++)
	{
		m1[i] = (int*)malloc(tam*sizeof(int));
		m2[i] = (int*)malloc(tam*sizeof(int));
		if(m1[i] == NULL || m2[i] == NULL)
		{
			printf("No se ha podido reservar espacio en memoria\n");
			exit(1);
		}
	}
}

void LiberaMemoria(int **m1, int **m2, int tam)
{
	int i;
	for(i = 0; i < tam; i++)
	{
		free(m1[i]);
		free(m2[i]);
	}
}


void cargaArchivo(FILE *FArch, int **m, int tam, int numMatriz)
{
	int j = 0, i = 0, elemento;
	int aux;
	char cCar; 
	while (feof(FArch) == 0)
	{
		cCar = fgetc(FArch);
		if(cCar != '\n' && cCar != '\0')
		{	
			elemento = cCar - '0';
			if(elemento >= 0 && elemento <= 9)
			{
				if(j < tam)
				{
					if(numMatriz == 1)
						m[i][j] = elemento;
					else if(numMatriz == 2)
						m[j][i] = elemento;
					
					j++;
				}
				else
				{
					j = 0;
					i++;

					if(numMatriz == 1)
						m[i][j] = elemento;
					else if(numMatriz == 2)
						m[j][i] = elemento;
					
					j++;
				}
			}
		}
	}
	fclose(FArch);
}
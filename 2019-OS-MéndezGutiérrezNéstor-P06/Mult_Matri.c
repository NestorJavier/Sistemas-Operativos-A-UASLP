#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>//libreria de hilos
#include <string.h>
/****************************************************
*													*
*	Profesor:	Marcela Ortiz Hernández				*
*	Fecha:		02 de Octubre de 2019				*
*													*
*	Alumnos:	Hernández Pérez Priscila Itzel		*
*				Méndez Gutiérrez Néstor Javier		*
*				Franco Tenorio Roberto Carlos		*
*													*
****************************************************/
struct Ren_Col_struct//
{
	//Apuntadores a las matrices
	int **m1;
	int **m2;

	//Tamaño N de las matrices
	int longitud;

	//Numero de renglon de m1 y columna de m2 que el hilo va a procesar
	int iRen;
	int iCol;

	//Elemento resultante de las operaciones
	int iElemento;
};

void LiberaMemoria(int **m1, int **m2, int **mRes, int tam);
void ReservaMemoria(int **m1, int **m2, int **mRes, int tam);
void abreArchivo(FILE *FArch1, FILE *FArch2, char *cNomArch1, char *cNomArch2);
void imprimeMatriz(int **m, int tam, int numMat);
void cargaArchivo(FILE *FArch, int **m, int tam, int numMatriz);
int validaMatrices(FILE *FArch1, FILE *FArch2);

void * operacion_Ren_Col(void*); //Función que procesa la información de ambas matrices

int main(int argc, char *argv[])
{
	int tam, i, j, k = 0, Ren, Col, **mRes, **m1, **m2;
	FILE * FArch2 = fopen(argv[2], "rb");
	FILE * FArch1 = fopen(argv[1], "rb");
	
	if(argc != 3) //Se verifica que los argumentos sean 3
	{
		printf("Argumentos Incorrectos\n");
		exit(1);
	}

	abreArchivo(FArch1, FArch2, argv[1], argv[2]);
	tam = validaMatrices(FArch1, FArch2); //Valida que el tamaño de las matrices sea el mismo y regresa el tamaño
	
	//Reserva memoria para las matrices
	m1 = (int**)malloc(tam*sizeof(int*));
	m2 = (int**)malloc(tam*sizeof(int*));
	mRes = (int**)malloc(tam*sizeof(int*));

	if(m1 == NULL || m2 == NULL || mRes == NULL)
	{
		printf("No se ha podido reservar espacio en memoria\n");
		exit(1);
	}
	
	//Reserva memoria para los renglones de las matrices
	ReservaMemoria(m1, m2, mRes, tam);

	//Carga los archivos y guarda la información en las matrices dinamicas
	cargaArchivo(FArch1, m1, tam, 1);
	cargaArchivo(FArch2, m2, tam, 2);

	//////////////////////
	int iNumHilos = tam*tam;			//Numero de hilos
	printf("Hilos: %d\n", iNumHilos);
	printf("Padre %d\n", getpid());		//Obtiene el identificador del proceso que invoca a los hilos
	//////////////////////

	pthread_t tids[iNumHilos];				// crea un arreglo de hilos
	struct Ren_Col_struct info[iNumHilos];	//Estructura que contiene la información necesaria para el calculo del elemento de la matriz resultante
	
	for(i = 0; i < tam; i++)
	{
		for(j = 0; j < tam; j++)
		{
									//Datos para el calculo del elemento
			info[k].iRen = i;		//Indice coorrespondiente al renglon de la matriz A 
			info[k].iCol = j;		//Indice coorrespondiente a la columna de la matriz B
			info[k].iElemento = 0;	//Inicializamos el valor del elemento en 0
			info[k].m1 = m1;		//Apuntadores a ambas marices
			info[k].m2 = m2;		//
			info[k].longitud = tam;	//Tamaño N de las matrices
			
			pthread_create(&tids[k], NULL, operacion_Ren_Col, &info[k]);  //creación del hilo
			k++;
		}
	}

	//Esperar que los hilos terminen
	for(i = 0; i < iNumHilos; i++)
	{
		//Recorremos el arreglo de hilos esperando a que cada uno de ellos termine 
		pthread_join(tids[i], NULL);

		//El numero de renglon y columna de las matrices originales tambien coorresponden a los indices
		//del elemento calculado de la matriz resultante
		mRes[info[i].iRen][info[i].iCol] = info[i].iElemento;
		
	}

	printf("Matriz A:\n");
	imprimeMatriz(m1, tam, 1);
	printf("Matriz B:\n");
	imprimeMatriz(m2, tam, 2);
	printf("Matriz Resultante C:\n");
	imprimeMatriz(mRes, tam, 3);

	//Se libera la memoria usada para alamcenar la información procesada
	LiberaMemoria(m1, m2, mRes, tam);
	free(m1); 
	free(m2); 
	free(mRes);

	exit(0);	
}

void abreArchivo(FILE *FArch1, FILE *FArch2, char *cNomArch1, char *cNomArch2)
{
	FArch1 = fopen(cNomArch1, "rb");
	FArch2 = fopen(cNomArch2, "rb");

	if(FArch1 == NULL || FArch2 == NULL)
	{
		printf("error al abrir el archivo\n");
		exit(1);
	}
}

int validaMatrices(FILE *FArch1, FILE *FArch2)
{
	char tam1[4], tam2[4];
	int tam = 0, tm = 0;
	if(feof(FArch1) == 0 || feof(FArch2) == 0)
	{
		fgets(tam1, 4, FArch1);
		fgets(tam2, 4, FArch2);
		tm =  atoi(tam2);
		tam = atoi(tam1);
		if(tm == tam)
		{
			if(tam >= 2 && tam <= 20)
				printf("El tamaño de las matrices es el mismo y esta dentro del rango\n");
			else
			{
				printf("El tamaño de las matrices no esta dentro del rango\n");
				exit(1);
			}
		}
		else
		{
			printf("El tamaño de las matrices no es el mismo\n");
			exit(1);
		}
	}
	else
	{
		printf("El archivo esta vacio\n");
		exit(1);
	}
	return tam;
}

void imprimeMatriz(int **m, int tam, int numMat)
{
	int i, j;
	for(i = 0; i < tam; i++)
	{
		for(j = 0; j < tam; j++)
		{
			if(numMat != 2)
				printf("%d ",m[i][j]);
			else
				printf("%d ",m[j][i]);
		}
		printf("\n");
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
			else
			{
				printf("El elemento: %d no cumple con ser igual o mayor a 0 o menor o igual a 9\n", elemento);
			}
		}
	}
	fclose(FArch);
}

void * operacion_Ren_Col(void* argument)
{
	struct Ren_Col_struct* arg_struct = (struct Ren_Col_struct*)argument;
	int auxRen, auxCol, i;
	
	for(i = 0; i < arg_struct->longitud; i++)
	{
		auxRen = arg_struct->m1[arg_struct->iRen][i];
		auxCol = arg_struct->m2[arg_struct->iCol][i];

		arg_struct->iElemento = arg_struct->iElemento + (auxRen*auxCol);
	}	
	return (void*) pthread_self();
}

void ReservaMemoria(int **m1, int **m2, int **mRes, int tam)
{
	int i;
	for(i = 0; i < tam; i++)
	{
		m1[i] = (int*)malloc(tam*sizeof(int));
		m2[i] = (int*)malloc(tam*sizeof(int));
		mRes[i] = (int*)malloc(tam*sizeof(int));
		if(m1[i] == NULL || m2[i] == NULL || mRes[i] == NULL)
		{
			printf("No se ha podido reservar espacio en memoria\n");
			exit(1);
		}
	}
}

void LiberaMemoria(int **m1, int **m2, int **mRes, int tam)
{
	int i;
	for(i = 0; i < tam; i++)
	{
		free(m1[i]);
		free(m2[i]);
		free(mRes[i]);
	}
}
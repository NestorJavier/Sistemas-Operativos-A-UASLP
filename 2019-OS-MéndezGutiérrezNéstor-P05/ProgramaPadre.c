#include <stdio.h>
#include <stdlib.h>
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

void ReservaMemoria(int **m1, int **m2, int **mRes, int tam);
void cargaArchivo(FILE *FArch, int **m, int tam);
void imprimeMatriz(int **m, int tam);
void LiberaMemoria(int **m1, int **m2, int **mRes, int tam);



int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("Argumentos Incorrectos\n");
		exit(1);
	}
	
	int tam, **mRes, **m1, **m2, i, j, k, l, PID, tm;
	char tam1[4], tam2[4], Ren[7], Col[7];
	FILE * FArch1 = fopen(argv[1], "rb");
	FILE * FArch2 = fopen(argv[2], "rb");
	pid_t pid;

	if(FArch1 == NULL || FArch2 == NULL)
	{
		printf("error al abrir el archivo");
		exit(1);
	}

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
			
			m1 = (int**)malloc(tam*sizeof(int*));
			m2 = (int**)malloc(tam*sizeof(int*));
			mRes = (int**)malloc(tam*sizeof(int*));
			
			if(m1 == NULL || m2 == NULL || mRes == NULL)
			{
				printf("No se ha podido reservar espacio en memoria\n");
				exit(1);
			}
			
			ReservaMemoria(m1, m2, mRes, tam);
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

	cargaArchivo(FArch1, m1, tam);
	cargaArchivo(FArch2, m2, tam);
	int cont = 0;
	
	//////////////////////
	int NumProces = tam*tam;
	printf("Procesos: %d\n", NumProces);
	printf("Padre %d\n", getpid());
	pid = fork();
	//////////////////////
	for(i = 0; i < tam; i++)
	{
		sprintf(Ren, "%d", i);

		for(k = 0; k < tam; k++)
		{
			sprintf(Col, "%d", k);

			if(pid < 0)
			{
				printf("Error al crear el proceso\n");
			}
			else if(pid == 0)
			{
				char *args[] = {"./Calcula_RenCol", Ren, Col, argv[1], argv[2], NULL};
				execv(args[0], args);
			}
			else if (pid > 0)  //// esta es la condición que les faltaba, solo el padre  guarda el id del hilo en la matriz y crea un nuevo proceso hijo
			{
				mRes[i][k] = pid;
				
				//printf("Proceso hijo: %d, %d, %d\n", mRes[i][k], i, k);
				pid = fork();
			}
		}
	}
	
	if (pid > 0)	//FALTA ESTA CONDICION
	{
		int estado = 0, valorHijo = 0;
		for(i=0; i < NumProces; i++)
		{
			pid_t pid_hijo = wait(&estado);//El padre espera con esta función que su hijo termine
				//Termina con exit para que el padre se dé cunenta 
			valorHijo = WEXITSTATUS(estado);//Macro que sirve para obtener el valor que me interesa 
							//de la estructura, es decir, el valor que regresa el exit
			for(j = 0; j < tam; j++)
			{
				for(k = 0; k < tam; k++)
				{
					if(mRes[j][k] == pid_hijo)
					{
						//printf("ID Mat: %d, Id a comparar: %d\n", mRes[j][k], pid_hijo);
						mRes[j][k] = valorHijo;
						break;
					}
				}
			}
			//printf("Proceso padre conoce que el proceso hijo %d termina su trabajo con el valor %d \n", pid_hijo, valorHijo);
		}
	}
	if (pid > 0)	//FALTA ESTA CONDICION
	{
		printf("Matriz A:\n");
		imprimeMatriz(m1, tam);
		printf("Matriz B:\n");
		imprimeMatriz(m2, tam);

		printf("Matriz Resultante C:\n");
		imprimeMatriz(mRes, tam);
	}

	LiberaMemoria(m1, m2, mRes, tam);
	free(m1); 
	free(m2); 
	free(mRes);	 
	exit(0);	
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


void cargaArchivo(FILE *FArch, int **m, int tam)
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
					m[i][j] = elemento;
					j++;
				}
				else
				{
					j = 0;
					i++;
					m[i][j] = elemento;
					j++;
				}
			}
		}
	}
	fclose(FArch);
}
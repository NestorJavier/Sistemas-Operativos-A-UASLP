// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

/****************************************************
*													*
*	Profesor:	Marcela Ortiz Hernández				*
*	Fecha:		30 de Octubre de 2019				*
*													*
*	Alumnos:	Hernández Pérez Priscila Itzel		*
*				Méndez Gutiérrez Néstor Javier		*
*				Franco Tenorio Roberto Carlos		*
*													*
****************************************************/

#include "copyright.h"
#include "system.h"
#define NUM 6
//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

int buffer[NUM];

void SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

void CapturaInfoArreglo()
{
	int i, numAux;
	printf("Ingrese 6 numeros:\n");	
	for(i = 0; i < NUM; i++)
	{
		scanf("%d", &numAux);
		buffer[i] = numAux;
	}
}

void bubbleSort(int arg)
{
    int i, it, aux;

    for(it = 1; it < NUM; it++)
    {
        for (i = 0; i < NUM-it; i++)
        {
            if(buffer[i] < buffer[i+1])
            {
              aux = buffer[i];
              buffer[i] = buffer[i+1];
              buffer[i+1] = aux;
            }
        }
    }
	printf("		Hilo 1\n");
	printf("		Elementos Ordenados\n");
	for(i = 0; i < NUM; i++)
		printf("		Elemento: %d, en la pos: %d\n", buffer[i], i);
	printf("\n");
	currentThread->Yield();
}

void Calculafactorial(int arg)
{
	int iNum = buffer[0];
	int i, fact, aux = 0;

	for(i = 0; i < NUM; i++)
	{
		if(aux < buffer[i])
			aux = buffer[i];
	}
	for(i = 1; i <= iNum; i++)
	{
		//Si es la primera i, teración el la variable fact es igual a 1
		if(i == 1)
			fact = i;
		else//Si no enonces el factorial es igual al factorial por la iteración
			fact = fact*i;
	}
	printf("		Hilo 2\n");
	printf("		El factorial es: %d\n\n", fact);

	currentThread->Yield();
}

void CalculaPromedio(int arg)
{
	int i;
	double prom, aux = 0;
	for(i = 0; i < NUM; i++)
		 aux += buffer[i];

	prom = (aux/NUM);

	printf("		Hilo 3\n");
	printf("		El promedio es: %f\n\n", prom);

	currentThread->Yield();
}

int verificaPrioridad(int p)
{
	int band = 0;
	if(p > 5 || p < 0)
	{
		printf("Ingrese un valor valido de 0 a 5 para la prioridad del hilo\n");
	}
	else
	{
		band = 1;
	}
	return band;
}


//----------------------------------------------------------------------
// ThreadTest
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest()
{
	int opcion;
	Thread *t1, *t2, *t3;
	int prio;
	DEBUG('t', "Entering SimpleTest");

	//Menu
	printf("		MENU\n");
	printf("		1.- SimpleThread\n");
	printf("		2.- Ejemplo de hilos (FCFS)\n");
	printf("		3.- Ejemplo de hilos (Prioridades no apropiativo)\n");
	printf("		4.- Exit\n");
	printf("		Opcion:");	
	scanf("%d", &opcion);//Variable donde se almacena la opción que eligo el usuario

	//Se verifica la opción del usuario
	switch(opcion)
	{
		case 1://En este caso se ejecuta el código original de nachos
		    t1 = new Thread("forked thread");
		    t2 = new Thread("forked thread");
		    t3 = new Thread("forked thread");
		    t1->Fork(SimpleThread, 1);
		    t2->Fork(SimpleThread, 2);
		    t3->Fork(SimpleThread, 3);
		break;
		case 2://En este caso se captura un arreglo el cual debe ser ordenado de forma decendente, se debe calcular el
			   //factorial al mayor elemento del arreglo y por ultimo calculra el promedio de los elementos del arreglo 
		    CapturaInfoArreglo();
			t1 = new Thread("forked thread");
		    t2 = new Thread("forked thread");
		    t3 = new Thread("forked thread");
		    t1->Fork(bubbleSort, 1);
		    t2->Fork(Calculafactorial, 2);
		    t3->Fork(CalculaPromedio, 3);
		break;
		case 3:
			CapturaInfoArreglo();
			printf("Prioridad para el hilo 1:\n");
			scanf("%d", &prio);
			while(verificaPrioridad(prio) != 1)
			{
				printf("Prioridad para el hilo 1:\n");
				scanf("%d", &prio);	
			}
			t1 = new Thread("forked thread", prio);

			printf("Prioridad para el hilo 2:\n");	
			scanf("%d", &prio);
			while(verificaPrioridad(prio) != 1)
			{
				printf("Prioridad para el hilo 2:\n");
				scanf("%d", &prio);	
			}
			t2 = new Thread("forked thread", prio);
			printf("Prioridad para el hilo 3:\n");	
			scanf("%d", &prio);
			while(verificaPrioridad(prio) != 1)
			{
				printf("Prioridad para el hilo 3:\n");
				scanf("%d", &prio);	
			}
			t3 = new Thread("forked thread", prio);

		    t1->Fork(bubbleSort, 1);
		    t2->Fork(Calculafactorial, 2);
		    t3->Fork(CalculaPromedio, 3);
		break;
		case 4:
		    printf("		Exit\n");
		break;
	}
}
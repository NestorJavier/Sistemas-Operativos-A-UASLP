void
bubbleSort(int *arr)
{
    int i, it, aux;

    for(it = 1; it < MAX; it++)
    {
        for (i = 0; i < MAX-it; i++)
        {
            if(buffer[i] < buffer[i+1])
            {
              aux = buffer[i];
              buffer[i] = buffer[i+1];
              buffer[i+1] = aux;
            }
        }
    }
}

void
NumMayor(int *arr)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

void
Promedio(int *arr)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

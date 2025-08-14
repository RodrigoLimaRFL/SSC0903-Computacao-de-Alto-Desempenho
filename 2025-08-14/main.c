#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define NUM_THREADS_CREATED 3

int token = 0;
sem_t semaphors[NUM_THREADS_CREATED + 1];

void *increment_token(void * arg)
{
    // Como pegar o valor de void* arg
    int num_thread = *((int *) arg);
    // Espera seu semáforo liberar
    sem_wait(&semaphors[num_thread]);
    token++;
    printf("Token incrementado para %i\n", token);
    // Libera a prox thread
    sem_post(&semaphors[num_thread+1]);
}

int main()
{
    pthread_t threads[NUM_THREADS_CREATED];

    // vetor para armazenar o numero das threads
    int args[NUM_THREADS_CREATED];

    for(int i = 0; i < NUM_THREADS_CREATED; i++)
    {
        sem_init(&semaphors[i], 0, 0);
        args[i] = i;
        // O argumento passado é região crítica
        // Logo é necessário armazenar em um vetor args[i]
        // Pois poderia pegar o valor 'i' errado
        if(pthread_create(&threads[i], 0, (void*) increment_token, (void*) &args[i]) != 0)
        {
            printf("ERRO AO CRIAR A THREAD\n");
            break;
        }
    }

    token = 1;
    printf("Token incrementado para %i \n", token);
    // Libera o primeiro semaforo
    sem_post(&semaphors[0]);
    // Espera liberar a main thread
    sem_wait(&semaphors[NUM_THREADS_CREATED]);
    printf("Token incrementado até %i, encerrando programa...\n", token);

    for(int i = 0; i < NUM_THREADS_CREATED; i++)
    {
        pthread_join(threads[i], 0);
    }

    return 0;
}
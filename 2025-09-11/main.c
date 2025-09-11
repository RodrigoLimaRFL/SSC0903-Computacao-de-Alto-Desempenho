// Fazer um anel de tasks dependentes para cada vez que incrementar token

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS 4 // Número de threads no paralelismo
#define NUM_TASKS 4 // Número de tasks por iteração

int token = 0;
int task_token = 0;

void create_tasks()
{
    int num_task;
    for (num_task = 0; num_task < NUM_TASKS; num_task++)
    {
        // inout => task depende da anterior e a próxima depende dela
        #pragma omp task depend(inout: num_task) 
        {
            // task_token é uma variável compartilhada, então precisamos de uma seção crítica
            #pragma omp critical (task_critical_section)
            {
                task_token++;
                printf("Task Token incrementado para %i\n", task_token);
            }
        }
    }
}

int main()
{
    int i;
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        #pragma omp single
        {
            printf("Executando com %d threads\n", omp_get_num_threads());
        }
        // Seção crítica para incrementar o token
        #pragma omp critical (token_increment_section)
        {
            token++;
            printf("Token incrementado para %i \n", token);
        }
        // Apenas uma thread cria as tasks
        #pragma omp single
        {
            create_tasks();
        } // Barreira implícita aqui, tasks são executadas neste ponto
    }

    return 0;
}
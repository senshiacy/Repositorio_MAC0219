#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 8

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Inicializa o mutex

int soma = 0;

struct thread_data{
    int thread_id;
    int sum;
};

struct thread_data thread_data_array[NUM_THREADS];

void *Executa(void *threadarg){

    sleep(1);

    pthread_mutex_lock(&mutex);

    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;

    int taskid = my_data -> thread_id;

    for(int i = 0; i <= taskid; i++){
        soma += i;
    }

    printf("Thread %d: A soma dos %d primeiros números é: %d\n", taskid, taskid, soma);
    soma = 0;

    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(){
    pthread_t threads[NUM_THREADS];
    int *taskids[NUM_THREADS];

    for(int t = 0; t < NUM_THREADS; t++){

        // Setar o objeto
        thread_data_array[t].thread_id = t;

        printf("Creating thread %d\n", t);

        int rc = pthread_create(&threads[t], NULL, Executa, (void *) &thread_data_array[t]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

        // Espera todas as threads terminarem
    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    pthread_mutex_destroy(&mutex);

    pthread_exit(NULL);
}
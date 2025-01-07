/*
    Importação de bibliotecas
*/

extern "C"{
    #include <pthread.h>
}
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

/*
    Criação de Variáveis Globais
*/

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;
vector<long> rochas;
long M, N, threshold, cont = 0;

/*
    Funções Auxiliares
*/

void swap(long i, long j){
    long tmp = rochas[i];
    rochas[i] = rochas[j];
    rochas[j] = tmp;
}

/*
    Funções para execução das Threads
*/

void *sapos(void *id){
    long atual = (long) id; // Inicializa com o id atual

    pthread_barrier_wait(&barrier);
    
    while(true){
        
        pthread_mutex_lock(&mutex); // inicializa a seção crítica
        
        if(atual - 1 >= 0 && rochas[atual - 1] == 0){ // Se há uma pedra para à esquerda
            swap(atual, atual - 1);
            atual--;
            cont = 0;
        }
        else if(atual - 2 >= 0 && rochas[atual - 2] == 0){ // Se há uma pedra para duas casas à esquerda
            swap(atual, atual - 2);
            atual -= 2;
            cont = 0;
        }
        else{ // Não consegue se mover
            cont++;
        }
        
        if(cont > threshold){ // Atingiu o limite superior
            pthread_mutex_unlock(&mutex);
            break;
        }
        
        pthread_mutex_unlock(&mutex);

        usleep(100000);

    }
    pthread_exit(NULL);
}

void *ras(void *id){
    long atual = (long) id; // Inicializa com o id atual

    pthread_barrier_wait(&barrier); // inicializa a seção crítica

    while(true){

        pthread_mutex_lock(&mutex);
        
        if(atual + 1 < N + M + 1 && rochas[atual + 1] == 0){ // Se há uma pedra para à direita
            swap(atual, atual + 1);
            atual++;
            cont = 0;
        }
        else if(atual + 2 < N + M + 1 && rochas[atual + 2] == 0){ // Se há uma pedra para duas casas à direita
            swap(atual, atual + 2);
            atual += 2;
            cont = 0;
        }
        else{ // Não consegue se mover
            cont++;
        }
        
        if(cont > threshold){ // Atingiu o limite superior
            pthread_mutex_unlock(&mutex);
            break;
        }
    
        pthread_mutex_unlock(&mutex);

        usleep(100000);

    }
    pthread_exit(NULL);
}

/*
    Verificação e Resolução de Problemas
*/

long venceu(){
    for(int i = 0; i < M; i++){
        if(rochas[i] < N + 1){
            return 0;
        }
    }
    if(rochas[M] != 0){
        return 0;
    }
    return 1;
}

long solve(){
    // Setagem de variáveis
    long rc, t;

    // Setagem de vetores
    pthread_t threads[M + N];
    rochas.assign(M + N + 2, 0);

    // Definição do Threshold
    threshold = 2*max(M, N)*(max(M, N) + 2);

    // Setagem do mutex e da barreira
    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier, nullptr, N + M);

    // Aleatorizar as threads
    vector<long> indices(N + M + 1, 0);
    for(t = 0; t < N + M + 1; t++){
        indices[t] = t;
    }

    // Embaralhar os índices
    random_device rd;
    mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);

    // Criação das Threads
    for(t = 0; t < N + M + 1; t++){

        long i = indices[t];

        if(i < N){ // Rã
            rochas[i] = i + 1;
            
            rc = pthread_create(&threads[i], NULL, ras, (void *) i);
        }
        else if(i > N){ // Sapo
            rochas[i] = i;

            rc = pthread_create(&threads[i - 1], NULL, sapos, (void *) i);    
        }

        if(rc){
            printf("ERROR; return code from pthread create() is %ld\n", rc);
            exit(-1);
        }
    }

        // Lançamento das Threads
    for(t = 0; t < N + M; t++){
        pthread_join(threads[t], NULL);
    }

    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex);

    return venceu();

    pthread_exit(NULL);
}

/*
    Função Principal
*/

int main(){
    double T;
    long soma = 0;

    // Tela de inputamento
    printf("Insira um valor para N: ");
    scanf("%ld", &N);
    printf("Insira um valor para M: ");
    scanf("%ld", &M);
    printf("Insira um valor para o número de tentativas: ");
    scanf("%lf", &T);

    for(long i = 0; i < T; i++){
        soma += solve();
    }

    printf("A taxa de acertos com %ld rãs, %ld sapos e %ld tentativas é de %.4lf\n", N, M, (long) T, soma/T);
}
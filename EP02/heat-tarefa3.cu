#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <cuda.h>

//#define t 16
//#define b 16

#define WALL_TEMP 20.0
#define FIREPLACE_TEMP 100.0

#define FIREPLACE_START 3
#define FIREPLACE_END 7
#define ROOM_SIZE 10

#define BODY_TEMP 37
#define BODY_START_X 14
#define BODY_START_Y 30
#define BODY_END_X 34
#define BODY_END_Y 50

// Inicializador. Funciona.
void initialize(double *h, int n) {
    int fireplace_start = (FIREPLACE_START * n) / ROOM_SIZE;
    int fireplace_end = (FIREPLACE_END * n) / ROOM_SIZE;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == 0 || i == n - 1 || j == 0 || j == n - 1) {
                h[i*n + j] = (i == n - 1 && j >= fireplace_start && j <= fireplace_end) ? FIREPLACE_TEMP : WALL_TEMP;
            } else if (i >= BODY_START_X && i <= BODY_END_X && j >= BODY_START_Y && j <= BODY_END_Y) {
                h[i*n + j] = BODY_TEMP;
            } else {
                h[i*n + j] = 0.0;
            }
        }
    }

    return;
}

// Iteração de jacobi sequencial.
void jacobi_iteration(double *h, double *g, int n, int iter_limit) {
    for (int iter = 0; iter < iter_limit; iter++) {
        for (int i = 1; i < n - 1; i++) {
            for (int j = 1; j < n - 1; j++) {
                if (!(i >= BODY_START_X && i <= BODY_END_X && j >= BODY_START_Y && j <= BODY_END_Y))
                g[i*n + j] = 0.25 * (h[(i-1)*n + j] + h[(i+1)*n + j] + h[i*n + j-1] + h[i*n + j+1]);
            }
        }
        for (int i = 1; i < n - 1; i++) {
            for (int j = 1; j < n - 1; j++) {
                if (!(i >= BODY_START_X && i <= BODY_END_X && j >= BODY_START_Y && j <= BODY_END_Y))
                h[i*n + j] = g[i*n + j];
            }
        }
    }

    return;
}

// Kernel de jacobi
__global__ void jacobi_kernel(double *h, double *g, int n) {
    int i = blockIdx.y * blockDim.y + threadIdx.y + 1;
    int j = blockIdx.x * blockDim.x + threadIdx.x + 1;

    if (i < n-1 && j < n-1) {
        if (!(i >= BODY_START_X && i <= BODY_END_X && j >= BODY_START_Y && j <= BODY_END_Y))
        g[i*n + j] = 0.25 * (h[(i-1)*n +j] + h[(i+1)*n + j] + h[i*n + j-1] + h[i*n + j+1]);
    }

    return;
}

// Calcula tempo do sequencial
double calculate_elapsed_time(struct timespec start, struct timespec end) {
    double start_sec = (double)start.tv_sec * 1e9 + (double)start.tv_nsec;
    double end_sec = (double)end.tv_sec * 1e9 + (double)end.tv_nsec;

    return (end_sec - start_sec) / 1e9;
}

// Salva em arquivo o da CPU
void save_to_file_cpu(double *h, int n) {
    FILE *file = fopen("room-cpu.txt", "w");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fprintf(file, "%lf ", h[i*n + j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);

    return;
}

// Salva em arquivo o da GPU
void save_to_file_gpu(double *h, int n) {
    FILE *file = fopen("room-gpu.txt", "w");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fprintf(file, "%lf ", h[i*n + j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);

    return;
}

// Validador
void compareResults(double *cpu, double *gpu, int n) {
    double epsilon = 2*1e-2;
    for(int i = 0; i < n; i ++) {
        if (abs(cpu[i] - gpu[i]) > epsilon) {
            printf("Comparação de resultados falhou\n");
            exit(-1);
        }
    }
    printf("Comparação de resultados passou\n");
}


int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Uso: %s <número de pontos> <limite de iterações> <número de thread> <número de blocos>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int iter_limit = atoi(argv[2]);
    int t = atoi(argv[3]);
    int b = atoi(argv[4]);
    // ^^ Comentar depois

    // Aloca memória necessária
    double *h_gpu; cudaMalloc(&h_gpu, n*n * sizeof(double)); 
    double *g_gpu; cudaMalloc(&g_gpu, n*n * sizeof(double));
    double *h_resposta = (double *)malloc(n*n * sizeof(double));
    double *h_cpu = (double *)malloc(n*n * sizeof(double));
    double *g_cpu = (double *)malloc(n*n * sizeof(double));

    if (h_resposta == NULL || h_gpu == NULL || g_gpu == NULL || h_cpu == NULL || g_cpu == NULL) {
        fprintf(stderr, "Erro ao alocar memória para h_resposta, h_gpu, g_gpu, h_cpu ou g_cpu\n");
        exit(EXIT_FAILURE);
    }

    // Inicializa o vetor
    struct timespec start_time, end_time;
    initialize(h_resposta, n);
    memcpy(h_cpu, h_resposta, (n*n * sizeof(double)));         // Copiando o vetor do h_resposta para cpu_h



    /************************************** CPU *********************************************/
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        jacobi_iteration(h_cpu, g_cpu, n, iter_limit);
        clock_gettime(CLOCK_MONOTONIC, &end_time);

        double elapsed_time = calculate_elapsed_time(start_time, end_time);
        printf("Dados referentes à execução sequencial (CPU):\n");
        printf("Tempo de execução: %.9f ms\n", elapsed_time*1000);
    /****************************************************************************************/



    /************************************** GPU *********************************************/
        // Cria os eventos do CUDA para podermos ver o tempo de transferência
        cudaEvent_t start, stop, copy_start, copy_stop;
        cudaEventCreate(&start); cudaEventCreate(&stop); cudaEventCreate(&copy_start); cudaEventCreate(&copy_stop);

        // Coloca em time_host_to_device o tempo em ms da cópia do host pra gpu
        cudaEventRecord(copy_start);
        cudaMemcpy(h_gpu, h_resposta, (n*n * sizeof(double)), cudaMemcpyHostToDevice);
        cudaMemcpy(g_gpu, h_resposta, (n*n * sizeof(double)), cudaMemcpyHostToDevice);
        cudaEventRecord(copy_stop);
        cudaEventSynchronize(copy_stop);
        float time_host_to_device;
        cudaEventElapsedTime(&time_host_to_device, copy_start, copy_stop);


        ///////////////// -----------------------

        // PARTE IMPORTANTE: OS CÁLCULOS!!
        // 1.1. Guarda o começo
        cudaEventRecord(start);

        // 2. Faz as contas de fato
        dim3 threadsPerBlock(t,t);
        dim3 blocksPerGrid(b,b);
        for (int iter = 0; iter < iter_limit; iter++) {
            jacobi_kernel<<<blocksPerGrid, threadsPerBlock>>>(h_gpu, g_gpu, n);
            cudaDeviceSynchronize();

            double *temp = h_gpu;
            h_gpu = g_gpu;
            g_gpu = temp;
        }

        // 1.2. Coloca em time_host_to_device o tempo de execução do programa
        cudaEventRecord(stop);
        cudaEventSynchronize(stop);
        float gpu_time;
        cudaEventElapsedTime(&gpu_time, start, stop);

        ///////////////// -----------------------

        // Coloca em time_device_to_host o tempo em ms da cópia da gpu pra host
        cudaEventRecord(copy_start);
        cudaMemcpy(h_resposta, h_gpu, (n*n * sizeof(double)), cudaMemcpyDeviceToHost);
        cudaEventRecord(copy_stop);
        cudaEventSynchronize(copy_stop);
        float time_device_to_host;
        cudaEventElapsedTime(&time_device_to_host, copy_start, copy_stop);

        // Imprime os coisos
        printf("\nDados referentes à execução paralela (GPU):\n");
        printf("Tempo de GPU: %.9f ms\n", gpu_time);
        printf("Tempo de movimentação Host->Device: %.9f ms\n", time_host_to_device);
        printf("Tempo de movimentação Device->Host: %.9f ms\n\n", time_device_to_host);
    /****************************************************************************************/

    save_to_file_cpu(h_cpu, n);
    save_to_file_gpu(h_resposta, n);
    compareResults(h_cpu, h_resposta, n*n);

    free(h_resposta);
    free(h_cpu);
    free(g_cpu);
    cudaFree(h_gpu);
    cudaFree(g_gpu);

    return 0;
}
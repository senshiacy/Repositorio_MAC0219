#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include <mpi.h>

#define TINT 1.0

int compute_julia_pixel(int x, int y, int width, int height, float tint_bias, unsigned char *rgb) {

    // Verifica se as coordenadas são válidas
    if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) {
        fprintf(stderr, "Coordenadas inválidas (%d,%d) para um pixel em uma imagem de %d x %d\n", x, y, width, height);
        return -1;
    }

    // "Amplia" a visualização para mostrar uma área agradável do conjunto de Julia
    float X_MIN = -1.6, X_MAX = 1.6, Y_MIN = -0.9, Y_MAX = +0.9;
    float float_y = (Y_MAX - Y_MIN) * (float)y / height + Y_MIN;
    float float_x = (X_MAX - X_MIN) * (float)x / width + X_MIN;

    // Ponto que define o conjunto de Julia
    float julia_real = -.79;
    float julia_img = .15;

    // Número máximo de iterações
    int max_iter = 300;

    // Calcula a convergência da série complexa
    float real = float_y, img = float_x;
    int num_iter = max_iter;
    while ((img * img + real * real < 2 * 2) && (num_iter > 0)) {
        float xtemp = img * img - real * real + julia_real;
        real = 2 * img * real + julia_img;
        img = xtemp;
        num_iter--;
    }

    // Pinta o pixel com base no número de iterações usando uma coloração estilizada
    float color_bias = (float) num_iter / max_iter;
    rgb[0] = (num_iter == 0 ? 200 : -500.0 * pow(tint_bias, 1.2) * pow(color_bias, 1.6));
    rgb[1] = (num_iter == 0 ? 100 : -255.0 * pow(color_bias, 0.3));
    rgb[2] = (num_iter == 0 ? 100 : 255 - 255.0 * pow(tint_bias, 1.2) * pow(color_bias, 3.0));

    return 0;
}

int write_bmp_header(FILE *f, int width, int height) {

    unsigned int row_size_in_bytes = width * 3 + 
        ((width * 3) % 4 == 0 ? 0 : (4 - (width * 3) % 4));

    // Define todos os campos no cabeçalho do BMP
    char id[3] = "BM";
    unsigned int filesize = 54 + (int)(row_size_in_bytes * height * sizeof(char));
    short reserved[2] = {0,0};
    unsigned int offset = 54;

    unsigned int size = 40;
    unsigned short planes = 1;
    unsigned short bits = 24;
    unsigned int compression = 0;
    unsigned int image_size = width * height * 3 * sizeof(char);
    int x_res = 0;
    int y_res = 0;
    unsigned int ncolors = 0;
    unsigned int importantcolors = 0;

    // Escreve os bytes no arquivo, mantendo o controle do
    // número de "objetos" escritos
    size_t ret = 0;
    ret += fwrite(id, sizeof(char), 2, f);
    ret += fwrite(&filesize, sizeof(int), 1, f);
    ret += fwrite(reserved, sizeof(short), 2, f);
    ret += fwrite(&offset, sizeof(int), 1, f);
    ret += fwrite(&size, sizeof(int), 1, f);
    ret += fwrite(&width, sizeof(int), 1, f);
    ret += fwrite(&height, sizeof(int), 1, f);
    ret += fwrite(&planes, sizeof(short), 1, f);
    ret += fwrite(&bits, sizeof(short), 1, f);
    ret += fwrite(&compression, sizeof(int), 1, f);
    ret += fwrite(&image_size, sizeof(int), 1, f);
    ret += fwrite(&x_res, sizeof(int), 1, f);
    ret += fwrite(&y_res, sizeof(int), 1, f);
    ret += fwrite(&ncolors, sizeof(int), 1, f);
    ret += fwrite(&importantcolors, sizeof(int), 1, f);

    // Sucesso significa que escrevemos 17 "objetos" com êxito
    return (ret != 17);
}

int main(int argc, char *argv[]){
    // Inicialização do ambiente MPI
    MPI_Init(&argc, &argv);

    //MPI_Barrier(MPI_COMM_WORLD);
    //double start_program = MPI_Wtime();

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Recebe um n
    if(argc != 2){
        if (rank == 0)
            fprintf(stderr, "Uso: %s <altura n>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    int n = atoi(argv[1]);
    int height = n;
    int width = 2*n;
    int quant_linhas_por_processo = height/size;
    int restolho = height%size;

    // Setagem do Intervalo de atuação de cada processo
    int linha_inicio = rank*quant_linhas_por_processo;
    if (rank < restolho) {
        linha_inicio += rank;
    } else {
        linha_inicio += restolho;
    }

    int quant_linhas = quant_linhas_por_processo;
    if (rank < restolho) {
        quant_linhas += 1;
    }

    printf("[Process %d out of %d]: I should compute pixel rows %d to %d, for a total of %d rows\n",
        rank, size, linha_inicio, linha_inicio+quant_linhas-1, quant_linhas);

    // Setagem inicial da imagem
    size_t size_thread = 3*quant_linhas*width*sizeof(unsigned char);
    unsigned char *pixels_thread = (unsigned char*)malloc(size_thread);

    float tint_bias = TINT;

    // Timer 
    double start_time = MPI_Wtime();
    for(int j = 0; j < quant_linhas; j++){
        for(int i = 0; i < width; i++){
            size_t pixel_index = 3 * j * width + 3 * i;
            compute_julia_pixel(i, j+linha_inicio, width, height, tint_bias, pixels_thread+pixel_index);
        }
    }

    double end_time = MPI_Wtime();
    double tempo = end_time-start_time;
    
    MPI_Barrier(MPI_COMM_WORLD);

    // Impressão de Resultados
    FILE *output_file;
    if (rank == 0) {
        output_file = fopen("julia.bmp", "wb");
        write_bmp_header(output_file, width, height);
        fclose(output_file);
    }

    for (int p = 0; p < size; p++) {
        if (rank == p) {
            start_time = MPI_Wtime();
            for (int j = 0; j < quant_linhas; j++) {
                

                output_file = fopen("julia.bmp", "ab");
                fwrite(&(pixels_thread[3*j*width]), sizeof(char), 3*width, output_file);
                unsigned char padding[3] = {0, 0, 0};
                fwrite(padding, sizeof(char), (width*3) % 4, output_file);
                fclose(output_file);
                
            }
            end_time = MPI_Wtime();
            printf("[Process %d out of %d]: Executed for %f seconds\n", rank, size, tempo+end_time-start_time);
            //printf("%.8f,\n", tempo+end_time-start_time);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    free(pixels_thread);

    //double end_program = MPI_Wtime();
    //printf("Init: %.8f,\n", start_program);
    //printf("Fin: %.8f,\n", end_program);

    MPI_Finalize();

    return 0;
}

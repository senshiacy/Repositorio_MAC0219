#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define CHUNK 12
#define N 8

double c_x_min;
double c_x_max;
double c_y_min;
double c_y_max;

double pixel_width;
double pixel_height;

int iteration_max = 200;

int image_size;
unsigned char **image_buffer;

int i_x_max;
int i_y_max;
int image_buffer_size;

int gradient_size = 16;
int colors[17][3] = {
                        {66, 30, 15},
                        {25, 7, 26},
                        {9, 1, 47},
                        {4, 4, 73},
                        {0, 7, 100},
                        {12, 44, 138},
                        {24, 82, 177},
                        {57, 125, 209},
                        {134, 181, 229},
                        {211, 236, 248},
                        {241, 233, 191},
                        {248, 201, 95},
                        {255, 170, 0},
                        {204, 128, 0},
                        {153, 87, 0},
                        {106, 52, 3},
                        {16, 16, 16},
                    };

// Parte sequencial: Alocação da memória
void allocate_image_buffer(){
    int rgb_size = 3;
    image_buffer = (unsigned char **) malloc(sizeof(unsigned char *) * image_buffer_size);

    for(int i = 0; i < image_buffer_size; i++){
        image_buffer[i] = (unsigned char *) malloc(sizeof(unsigned char) * rgb_size);
    };
};

// Parte sequencial: Operação de I/O
void init(int argc, char *argv[]){
    if(argc < 6){
        printf("usage: ./mandelbrot_omp c_x_min c_x_max c_y_min c_y_max image_size\n");
        printf("examples with image_size = 11500:\n");
        printf("    Full Picture:         ./mandelbrot_omp -2.5 1.5 -2.0 2.0 11500\n");
        printf("    Seahorse Valley:      ./mandelbrot_omp -0.8 -0.7 0.05 0.15 11500\n");
        printf("    Elephant Valley:      ./mandelbrot_omp 0.175 0.375 -0.1 0.1 11500\n");
        printf("    Triple Spiral Valley: ./mandelbrot_omp -0.188 -0.012 0.554 0.754 11500\n");
        exit(0);
    }
    else{
        sscanf(argv[1], "%lf", &c_x_min);
        sscanf(argv[2], "%lf", &c_x_max);
        sscanf(argv[3], "%lf", &c_y_min);
        sscanf(argv[4], "%lf", &c_y_max);
        sscanf(argv[5], "%d", &image_size);

        i_x_max           = image_size;
        i_y_max           = image_size;
        image_buffer_size = image_size * image_size;

        pixel_width       = (c_x_max - c_x_min) / i_x_max;
        pixel_height      = (c_y_max - c_y_min) / i_y_max;
    };
};

void update_rgb_buffer(int iteration, int x, int y){
    int color;

    if(iteration == iteration_max){
        image_buffer[(i_y_max * y) + x][0] = colors[gradient_size][0];
        image_buffer[(i_y_max * y) + x][1] = colors[gradient_size][1];
        image_buffer[(i_y_max * y) + x][2] = colors[gradient_size][2];
    }
    else{
        color = iteration % gradient_size;

        image_buffer[(i_y_max * y) + x][0] = colors[color][0];
        image_buffer[(i_y_max * y) + x][1] = colors[color][1];
        image_buffer[(i_y_max * y) + x][2] = colors[color][2];
    };
};

// Parte sequencial: Operação de I/O
void write_to_file(){
    FILE * file;
    char * filename               = "output.ppm";
    char * comment                = "# ";

    int max_color_component_value = 255;

    file = fopen(filename,"wb");

    fprintf(file, "P6\n %s\n %d\n %d\n %d\n", comment,
            i_x_max, i_y_max, max_color_component_value);

    for(int i = 0; i < image_buffer_size; i++){
        fwrite(image_buffer[i], 1 , 3, file);
    };

    fclose(file);
};

// Única operação paralelizável
void compute_mandelbrot(){
    // 
    double z_x;
    double z_y;
    double z_x_squared;
    double z_y_squared;
    double escape_radius_squared = 4;

    // Iteradores
    int iteration;
    int i_x;
    int i_y;

    double c_x;
    double c_y;
    /*
        Para cada iteração i_y até i_y_max (definido pelo user):
            faça c_y <- c_y_min (definido pelo user) + i_y * pixel_height (definido pelo user)

            Para cada iteração i_x até i_x_max (definido pelo user):
                faça c_x <- c_x_min (definido pelo user) + i_x * pixel_width (definido pelo user)

                inicialize z_x = z_y = z_x_squared = z_y_squared = 0.0

                Para cada iteração iteration até a condição máxima ou \
                    ((z_x_squared + z_y_squared) < escape_radius_squared):

                    some z_y <- 2 * z_x * z_y + c_y;
                    some z_x <- z_x_squared - z_y_squared + c_x;
                    some z_x_squared <- z_x * z_x;
                    some z_y_squared <- z_y * z_y;

                    (Não vai ser impresso, é condição de parada)
                
                printa(A iteração, o i_x e o i_y)

    */
// 
    #pragma omp parallel private(i_y, i_x, iteration, c_x, c_y, z_x, z_y, z_x_squared, z_y_squared) num_threads(N)
    {
        #pragma omp for schedule(dynamic, CHUNK) nowait
		    for(i_y = 0; i_y < i_y_max; i_y++){ // i_y_max é global e recebe image_size
		        c_y = c_y_min + i_y * pixel_height; 
		        /* 
		            c_y_min é global = é o 3º argumento da argv
		            pixel_height é global e é (c_y_max - c_y_min) / i_y_max;
		        */

		        if(fabs(c_y) < pixel_height / 2){
		            c_y = 0.0;
		        };

		        for(i_x = 0; i_x < i_x_max; i_x++){
		            c_x         = c_x_min + i_x * pixel_width;
		            /*
		                c_x_min é global e é o 1º argumento de argv
		                pixel_width é global e é (c_x_max - c_x_min) / i_x_max;
		            */

		            z_x         = 0.0;
		            z_y         = 0.0;

		            z_x_squared = 0.0;
		            z_y_squared = 0.0;

		            //#pragma omp for schedule(dynamic, 12) //nowait
		            //{
		                for(iteration = 0;
		                    iteration < iteration_max && \
		                    ((z_x_squared + z_y_squared) < escape_radius_squared); // iteration_max é global e é 200
		                    iteration++){
		                    z_y         = 2 * z_x * z_y + c_y;
		                    z_x         = z_x_squared - z_y_squared + c_x;

		                    z_x_squared = z_x * z_x;
		                    z_y_squared = z_y * z_y;
		                };
		            //};
		            //#pragma omp critical
		            //{
		            	update_rgb_buffer(iteration, i_x, i_y);
		            //};
		        };
		    };
    };
};

int main(int argc, char *argv[]){
    init(argc, argv);

    allocate_image_buffer();
    
    // Obtenha os tempos sem I/O ou alocação de memória
    /*
    Parte Incial
    */
    //struct timespec start, stop;
  	//clock_gettime(CLOCK_MONOTONIC, &start);
  	
  	// Início da parte do código

    compute_mandelbrot();
    
   	// Fim da parte do código
    
    //clock_gettime(CLOCK_MONOTONIC, &stop);
    //printf("%lu,", (stop.tv_sec - start.tv_sec) * 1000000000 + (stop.tv_nsec - start.tv_nsec));
    /*
    Parte final
    */

    write_to_file();

    return 0;
};

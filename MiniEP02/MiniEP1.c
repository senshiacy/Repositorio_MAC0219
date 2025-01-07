/*
	Nathália Yukimi Uchiyama Tsuno
	14600541
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){

    // Setar as variáveis iniciais
    int nx = 500;
    double x0 = 0, t0 = 0;
    double xf = 10, tf = 10;

    // Definir vetores u, v, a e x de tamanho nx
    double u[nx];
    double v[nx];
    double a[nx];
    double x[nx];
    
    // Atribuir valores
    double deltax = (xf - x0)/(nx - 1);
    double deltat = deltax/2;
    double nt = ((tf - t0)/deltat) + 1;

    double j = x0;
    for(int i = 0; i < nx; i++){
        x[i] = j;
        j += deltax;
    }

    // Condições de contorno e iniciais
    for(int i  = 0; i < nx; i++){
        u[i] = exp(-((x[i]-5)*(x[i]-5)));
        v[i] = 0;
        a[i] = 0;
    }

    // Processamento
    double t = t0;
    double deltatleapfrog = 0;

    while(t <= tf){
        if(t == t0) {
            deltatleapfrog = deltat/2;
        }
        else {
            deltatleapfrog = deltat;
        }

        for(int i = 1; i < nx - 1; i++) {
            a[i] = (u[i - 1] + u[i + 1] - 2*u[i])/(deltax*deltax); // obtido através de diferenças finitas
        }

        for(int i = 0; i < nx; i++) {
            v[i] = v[i] + a[i] * deltatleapfrog;
        }

        for(int i = 0; i < nx; i++) {
            u[i] = u[i] + v[i] * deltat;
        }

        t += deltat;
    }

    // Imprima a solução para determinar convergência (verificando que 𝑢(5, 10) = −1)
    int i = (int) nx/2 - 10;
    while(i <= (int) nx/2 + 10){
        printf("%.16f %.16f\n", x[i], u[i]);
        i++;
    }
}

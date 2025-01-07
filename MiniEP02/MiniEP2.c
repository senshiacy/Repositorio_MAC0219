#include <stdio.h>
#include <stdlib.h>
#include <math.h>

signed main(){
	// Setar as variáveis iniciais
	// Definir vetores u, v, a e x de tamanho nx
	float u[500], v[500], a[500], x[500], deltax = 10.0f/ 499.0f, deltat = 5.0f / 499.0f, j = 0, deltatleapfrog = (5.0f/998.0f), doubledeltax = (100.0f/249001.0f);
	
    // Atribuir valores
    short int i = 0;
    
    for(i = 0; i < 500; i++){
    	x[i] = j;
    	j += deltax;
    }
    
    // Condições de contorno e iniciais
    for(i  = 0; i < 500; i++){
    	j = x[i] - 5.0f;
        u[i] = exp(-(j*j));
        v[i] = 0.0f;
        a[i] = 0.0f;
    }
    
    // Processamento
	for(i = 1; i < 499; i++) {
		a[i] = (u[i - 1] + u[i + 1] - 2*u[i])/doubledeltax; // obtido através de diferenças finitas
	}

	for(i = 0; i < 500; i++) {
		v[i] = v[i] + a[i] * deltatleapfrog;
		u[i] = u[i] + v[i] * deltat;
	}
	
	for(j = 1; j <= 998; j++){
		for(i = 1; i < 499; i++) {
			a[i] = (u[i - 1] + u[i + 1] - 2*u[i])/doubledeltax; // obtido através de diferenças finitas
		}
		
		for(i = 0; i < 500; i++) {
			v[i] = v[i] + a[i] * deltat;
			u[i] = u[i] + v[i] * deltat;
		}
	}
	
    // Cria um buffer para armazenar a saída
    char buffer[1024];
    short int pos = 0;
    
    // Adiciona os resultados ao buffer
    for (i = 240; i <= 260; i++) {
        pos += snprintf(buffer + pos, sizeof(buffer) - pos, "%.16f %.16f\n", x[i], u[i]);
    }
    
    // Imprime todos os resultados de uma vez
    printf("%s", buffer);


    return 0;
}

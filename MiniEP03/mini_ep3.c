#include <stdio.h>
#include <stdlib.h>

/**
void slowsort(int arr[], int i, int j) {
	// Base da recursão	
    if (i >= j) return;
    
    // Divisão e conquista
    // Encontra o meio do array e divide-o em duas partes disjuntas
    int m = (i + j) / 2;
    slowsort(arr, i, m);
    slowsort(arr, m + 1, j);
    
    // Ordena o elemento do meio com o último
    if (arr[j] < arr[m]) {
        int temp = arr[j];
        arr[j] = arr[m];
        arr[m] = temp;
    }
    
    // Continua a recursão
    slowsort(arr, i, j - 1);
}
**/

void partition(int arr[], int lo, int hi, int *lt, int *gt){
	int p = arr[lo], i = lo, j = hi, k = lo;
	
	while(k <= j){
		if(arr[k] < p){
			int tmp = arr[i];
			arr[i] = arr[k];
			arr[k] = tmp;
			i++;
			k++;
		}
		else if(arr[k] > p){
			int tmp = arr[k];
			arr[k] = arr[j];
			arr[j] = tmp;
			j--;
		}
		else{
			k++;
		}
	}
	
	*lt = i;
	*gt = j;
}

void quicksort(int arr[], int lo, int hi){
	if(lo < hi){
		int lt, gt;
		partition(arr, lo, hi, &lt, &gt);
		
		quicksort(arr, lo, lt - 1);
		quicksort(arr, gt + 1, hi);
	}
}

int fibonacci(int n, int *fib_memo) {
	if(fib_memo[n] != -1){
    	return fib_memo[n];
    }
    
    fib_memo[n] = fibonacci(n - 1, fib_memo) + fibonacci(n - 2, fib_memo);
    return fib_memo[n];
}

int main() {
	/** Setagem de valores **/
	
    const int TAMANHO = 200; // Tamanho do vetor
    const int SEED = 42; // Semente para números aleatórios
    int vetor[TAMANHO];
    int soma = 0;
    int tot = (2*2 + 2*2*2*2*2) + 1;
    
    // Otimização 1
    int *fib_memo = (int *)malloc((tot)*sizeof(int));
    
    for(int i = 5; i < tot; i++){
    	fib_memo[i] = -1;
    }
    fib_memo[0] = 0;
    fib_memo[1] = 1;
    fib_memo[2] = 1;
    fib_memo[3] = 2;
    fib_memo[4] = 3;
    
    /** Aleatorização do vetor **/

    // Inicializa o gerador de números aleatórios
    srand(SEED);

    // Preenche o vetor com números aleatórios entre 2^2 e (2^2 + 2^5) - 1
    for (int i = 0; i < TAMANHO; i++) {
        vetor[i] = (rand() % (1 << 5)) + (1 << 2);
    }
    
    /** Ordenação do vetor **/

    // Ordena o vetor usando Slowsort
    //slowsort(vetor, 0, TAMANHO - 1);
    quicksort(vetor, 0, TAMANHO - 1); // O(nlg(n)) 
    
    /** Calculo fibonnacico **/

    // Calcula o Fibonacci para cada elemento e soma os pares
    for (int i = 0; i < TAMANHO; i++) {
    	// Redução de chamadas à fibonacci
    	int fib;
    	if(fib_memo[vetor[i]] != -1){
    		fib = fib_memo[vetor[i]];
    	}
    	else{
    		fib = fibonacci(vetor[i], fib_memo);
    	}
    	
        if ((fib & 1) == 0) { // Operação bitwise para tentar mitigar os custos do resto
            soma += fib;
        }
    }

    printf("Soma dos números de Fibonacci pares: %d\n", soma);
    
    free(fib_memo);

    return 0;
}

/** Otimização 1 - Memorização para Fibonacci
		Conforme o gprof, fibonnaci é altamente recursivo. 
		Com a dp memoizada, conseguimos
		Reduzir o número de chamadas recursivas.
**/

/** Otimização 1.1 - Pré-cômputo de alguns valores
		O pré-cômputo de alguns valores fibonaccicos iniciais
		pode auxiliar no desempenho algorítmico, uma vez que não precisa 
		de certas chamadas recursivas.
**/
/** Otimização 1.2 - Verificação de valores pré-calculados
		A chamada às funções é uma operação que tem seu custo.
		Então, antes de chamar fibonacci, verificamos se 
		o valor já não foi pré-computado.
**/

/** Otimização 2 - Melhorias do slowsort
		Conforme o gprof, slowsort consome cerca de 83% dos
		recursos. Altamente recursivo e demorado.
**/

/**	Otimização 3 - Mitigação do custo do resto
		Um método interessante é o uso de operações bitwise
		nos cálculos. Um deles, é na
		aplicação do encontro de valores pares. 
**/

/** Otimização 4 - Algoritmo de ordenação O(nlg(n))
		SlowSort é pior do que o BubbleSort no melhor caso.
		Então, vamos substituí-lo pelo QuickSort que faz o
		mesmo papel por um custo MUITO menor.
**/

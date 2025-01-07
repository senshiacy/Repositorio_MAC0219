# MiniEP06 - Múltiplicação de Matrizes e Cache

Numa breve descrição, esse mini ep foca na multiplicação de matrizes e otimizações de memória cache. As otimizações requiridas e executadas foram a otimização de localidade de acesso e o uso da técnica de blocagem.

## Execução 

Para executar o código:

```bash
make
./main <ARGS>
```

> onde:  
   --matrix-size <NUM>     Tamanho da matriz quadrada. (n_linhas = n_colunas)  
    --algorithm <NUM>       Número da implementação. (0 = dgemm_0, 1 = dgemm_1, 2 = dgemm_2)
# EP01 - Conjunto de Mandelbrot

Numa breve descrição, esse ep visa a aplicação de conhecimentos e análises das complexidades
em programas paralelizados com Pthreads e OMP.

## Dependências

As bibliotecas $\textcolor{#fb6066}{POSIX\text{ }Threads}$ e $\textcolor{#fb6066}{OpenMP}$ já
estão inclusas no gcc.

As versões utilizadas foram:

```bash
ldd 2.31 (Para Pthreads)
OpenMP 4.5 (Para OMP)
gcc 11.4.0
```

## Execução

Para compilar os programas:

```bash
make
```

Para executar a versão sequencial:

```bash
./mandelbrot_seq <ARGS>
```

> Sob o uso: ./mandelbrot_seq c_x_min c_x_max c_y_min c_y_max image_size

Para executar a versão paralelizada com Pthreads:

```bash
./mandelbrot_pth <ARGS>
```

> Sob o uso: ./mandelbrot_seq c_x_min c_x_max c_y_min c_y_max image_size

Para executar a versão paralelizada com OMP:

```bash
./mandelbrot_omp <ARGS>
```

> Sob o uso: ./mandelbrot_seq c_x_min c_x_max c_y_min c_y_max image_size

## Resultados

As análises estão inclusas no documento *MAC0219___Relatório_EP01.pdf*
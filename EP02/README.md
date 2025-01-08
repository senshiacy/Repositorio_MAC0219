# EP02 - Equação de Calor e CUDA

Numa breve descrição, esse ep visa a aplicação da programação em CUDA e em GPUs.

## Dependências

A priori, é necessário um ambiente que possibilite a programação em GPU's. Nosso
projeto foi programado sobre a máquina:

| | |
:-: | :-:
CPU | AMD Ryzen 7 5700G with Radeon Graphics
RAM | 125Gi
SO | Debian GNU/Linux 12 (bookworm)
Placa de Vídeo | NVIDIA GeForce RTX 3060

Em seguida, é necessário instalar o [CUDA Toolkit](https://developer.nvidia.com/cuda-toolkit).

Nossas especificações foram:

> CUDA Toolkit, versão 11.8  
> nvcc, versão 11.8.89

## Execução

Para executar a versão sequencial:

```bash
make heat-tarefa1
./heat-tarefa1
```

Para executar as versões paralelizadas:

```bash
make heat-tarefa2
make heat-tarefa3
./heat-tarefa2
./heat-tarefa3
```

## Resultados

As análises estão disponíveis em *MAC0219___Relatório_EP02.pdf*

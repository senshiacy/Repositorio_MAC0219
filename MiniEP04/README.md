# MiniEP04 - Problemas de Concorrência

Numa breve descrição, o código ilustra problemas de concorrência devido à atomicidade.

## Execução

Para executar a versão sequencial:

```bash
gcc MiniEP4.c -o MiniEP4 -pthread
./MiniEP4
```

Para executar a versão problemática:

```bash
gcc MiniEP4Local.c -o MiniEP4Local -pthread
./MiniEP4Local
```

Para executar a versão coerente e correta:

```bash
gcc MiniEP4Mutex.c -o MiniEP4Mutex -pthread
./MiniEP4Mutex
```
#NVCC = /usr/local/cuda/bin/nvcc
NVCC = /usr/lib/nvidia-cuda-toolkit/bin/nvcc
CUDAPATH = /usr/lib/nvidia-cuda-toolkit/

NVCCFLAGS = -I$(CUDAPATH)/include
LFLAGS = -L$(CUDAPATH)/lib64 -lcuda -lcudart -lm

heat-tarefa1:
	$(NVCC) $(NVCCFLAGS) $(LFLAGS) -o heat-tarefa1 heat-tarefa1.c

heat-tarefa2:
	$(NVCC) $(NVCCFLAGS) $(LFLAGS) -o heat-tarefa2 heat-tarefa2.cu

heat-tarefa3:
	$(NVCC) $(NVCCFLAGS) $(LFLAGS) -o heat-tarefa3 heat-tarefa3.cu

clean:
	rm -f heat-tarefa1 heat-tarefa2 heat-tarefa3

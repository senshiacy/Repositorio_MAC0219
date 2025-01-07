NVCC = /lib/nvidia-cuda-toolkit/bin/nvcc
CUDAPATH = /lib/cuda

NVCCFLAGS = -I$(CUDAPATH)/include
LFLAGS = -L$(CUDAPATH)/lib64 -lcuda -lcudart -lm

VectorAdd:
	$(NVCC) $(NVCCFLAGS) $(LFLAGS) -o VectorAdd VectorAdd.cu

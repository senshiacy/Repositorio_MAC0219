TARGET = sequential_julia
CC = gcc
CFLAGS = -Wall -Wextra -O2
CFLAGSEXTRA = -lm

all: $(TARGET)

$(TARGET): sequential_julia.o
	$(CC) $(CFLAGS) -o $(TARGET) sequential_julia.o $(CFLAGSEXTRA)

sequential_julia.o: sequential_julia.c 
	$(CC) $(CFLAGS) -c sequential_julia.c 

clean:
	rm -f $(TARGET) sequential_julia.o

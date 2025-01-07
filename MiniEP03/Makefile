# Definições das variáveis
TARGET = mini_ep3
SOURCES = mini_ep3.c
CFLAGS = -Wall -g -pg

# Regra padrão
all: $(TARGET)

# Regra para compilar o programa
$(TARGET): $(SOURCES)
	gcc $(CFLAGS) -o $@ $^
	
# Regra para compilar para usar com perf
$(TARGET)_perf: $(SOURCES)
	gcc $(CFLAGS) -o $@ $^

# Regra para limpar arquivos gerados
clean:
	rm -f $(TARGET) gmon.out perf.data

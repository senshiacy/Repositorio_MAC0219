# Nome do compilador
CXX = g++

# Opções de compilação
CXXFLAGS = -Wall -pthread

# Nome do executável
TARGET = mini_ep5

# Arquivos fonte
SRCS = mini_ep5.cpp

# Regras de compilação
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
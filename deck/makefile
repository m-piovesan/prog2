# Nome do programa gerado
TARGET = meu_programa

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -g

# Pasta onde estão os arquivos de cabeçalho (headers)
INCLUDES = -I.

# Arquivos fonte
SRCS = libdeck.c main.c

# Objetos gerados
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

# Regra para compilação do programa
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS)

# Regra para compilação dos objetos
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
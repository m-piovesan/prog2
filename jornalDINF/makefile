# Nome do programa gerado
TARGET = jornalDINF

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -g

# Pasta onde estão os arquivos de cabeçalho (headers)
INCLUDES = -I.

# Arquivos fonte
SRCS = libjornal.c main.c

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
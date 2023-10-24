CC = gcc
CFLAGS = -Wall
TARGET = space
SRCS = A3.c enemy.c space.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
	find . -type f -name "*.txt" -exec rm -f {} \;

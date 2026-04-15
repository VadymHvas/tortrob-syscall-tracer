CC = gcc
CFLAGS = -Wall -gdwarf -g3 -O0 -Iinclude
LDFLAGS =

OUTPUT = ttrace

SRCS := $(shell find . -name "*.c")

OBJS := $(SRCS:.c=.o)

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) $(OBJS) -o $(OUTPUT) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OUTPUT)

.PHONY: all clean
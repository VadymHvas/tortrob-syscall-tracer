CC = gcc
CFLAGS = -Wall -gdwarf -g3 -O0 -Iinclude
LDFLAGS =

SRCS = main.c parser/opt.c parser/syscall.c parser/syscalls/ABI/abi.c parser/syscalls/parser.c \
	parser/syscalls/fs.c core/trace.c core/process.c core/dispatch.c

OBJS = $(SRCS:.c=.o)

OUTPUT = ttrace

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) $(OBJS) -o $(OUTPUT) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OUTPUT)

.PHONY: all clean
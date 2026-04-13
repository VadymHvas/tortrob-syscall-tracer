CC = gcc
CFLAGS = -Wall -gdwarf -g3 -O0 -Iinclude
LDFLAGS =

SRCS = main.c parser/opt.c core/trace.c core/process.c core/dispatch.c \
	parser/syscall.c parser/syscalls/ABI/abi.c parser/syscalls/parser.c \
	parser/syscalls/fs.c parser/syscalls/args/helpers.c parser/syscalls/args/struct_info.c \
	parser/syscalls/args/value_info.c parser/syscalls/args/flag_info.c \
	parser/syscalls/args/fs/flags.c parser/syscalls/args/fs/mask.c parser/syscalls/args/fs/special.c \
	parser/syscalls/args/fs/struct.c parser/syscalls/args/fs/value.c

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
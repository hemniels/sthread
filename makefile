CC=gcc
CFLAGS=-Wall -g

# List of source files
SRCS=main.c scheduler/sthread.c

# List of object files
OBJS=$(SRCS:.c=.o)

# Output executable
EXECUTABLE=main

# Main target
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(EXECUTABLE)

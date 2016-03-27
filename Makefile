CC = gcc
CFLAGS = -Wno-unused -Wno-unused-macros
V = @
EXECUTABLE = shell

all: shell.o parser.o
	$(V)$(CC) $(CFLAGS) shell.c parser.c -o shell

shell.o: shell.c 
	@echo + shell.c
	$(V)$(CC) $(CFLAGS) -c shell.c

parser.o: parser.c 
	@echo + parser.c
	$(V)$(CC) $(CFLAGS) -c parser.c

clean:
	$(V)rm parser.o shell.o shell
	make all

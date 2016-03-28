CC = gcc
CFLAGS = -Wno-unused -Wno-unused-macros -g
V = @
EXECUTABLE = shell

all: shell.o parser.o executer.o
	$(V)$(CC) $(CFLAGS) shell.c parser.c executer.c -o shell

shell.o: shell.c 
	@echo + shell.c
	$(V)$(CC) $(CFLAGS) -c shell.c

parser.o: parser.c 
	@echo + parser.c
	$(V)$(CC) $(CFLAGS) -c parser.c

executer.o: executer.c 
	@echo + executer.c
	$(V)$(CC) $(CFLAGS) -c executer.c

clean:
	$(V)rm parser.o shell.o executer.o shell
	$(V)make all

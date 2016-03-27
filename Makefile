CC = gcc
CFLAGS = -Wno-unused -Wno-unused-macros
V = @

all:
	@echo + shell.c
	$(V)$(CC) $(CFLAGS) shell.c -o shell

CC = g++
CFLAGS = -g -W -Wall -Werror -Wno-unused
V = @

all:
	@echo + shell.cpp
	$(V)$(CC) $(CFLAGS) shell.cpp -o shell

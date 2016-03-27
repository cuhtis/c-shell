CC = clang++
CFLAGS = -std=c++11 -stdlib=libc++ -Weverything -Wno-unused -Wno-unused-macros
V = @

all:
	@echo + shell.cpp
	$(V)$(CC) $(CFLAGS) shell.cpp includes/*.cpp -o shell

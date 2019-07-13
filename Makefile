CFLAGS=-std=iso9899:1990
WARN_FLAGS=-Wall -Wextra -Wpedantic -Werror
CLANG=clang -Wassign-enum -Wenum-conversion
SANITIZE=-fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined

OBJS=candymatch

release: OPTIM_FLAGS=-Os -ffast-math
release: run

debug: OPTIM_FLAGS=-Og -ggdb -DDEBUG $(SANITIZE)
debug: run

valgrind: OPTIM_FLAGS=-Og -ggdb -DDEBUG
valgrind: run

CC=$(CLANG)

run: $(OBJS)
	./candymatch

candymatch:
	$(CC) -o candymatch candymatch.c -lSDL2

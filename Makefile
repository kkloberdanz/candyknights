CFLAGS=-std=c99
WARN_FLAGS=-Wall -Wextra -Wpedantic -Werror
LFLAGS=-lSDL2 -lSDL2_image
SANITIZE=-fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined

OBJS=candymatch

release: OPTIM_FLAGS=-Os -ffast-math
release: run

debug: OPTIM_FLAGS=-Og -ggdb -DDEBUG $(SANITIZE)
debug: run

valgrind: OPTIM_FLAGS=-Og -ggdb -DDEBUG
valgrind: run

CLANG=clang -Wassign-enum -Wenum-conversion
CC=$(CLANG) $(OPTIM_FLAGS) $(CFLAGS) $(WARN_FLAGS) $(LFLAGS)

run: $(OBJS)
	./candymatch

candymatch:
	$(CC) -o candymatch candymatch.c -lSDL2

clean:
	rm -f candymatch
	rm -f core

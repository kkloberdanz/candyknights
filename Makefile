STDFLAG=-std=c99
WARN_FLAGS=-Wall -Wextra -Wpedantic -Werror
LFLAGS=-lSDL2 -lSDL2_image -lm
SANITIZE=-fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined

OBJS=candymatch

release: OPTIM_FLAGS=-Os -ffast-math
release: run

debug: OPTIM_FLAGS=-Og -ggdb -DDEBUG $(SANITIZE)
debug: run

valgrind: OPTIM_FLAGS=-Og -ggdb3 -DDEBUG
valgrind: OTHER_RUNTIME=valgrind --leak-check=full
valgrind: run

CFLAGS=$(OPTIM_FLAGS) $(WARN_FLAGS) $(LFLAGS)
CLANG=clang -Wassign-enum -Wenum-conversion
GCC=gcc

CC=$(CLANG)

run: $(OBJS)
	$(OTHER_RUNTIME) ./candymatch

strip: $(OBJS)
	strip candymatch

candymatch:
	$(CC) -o candymatch candymatch.c $(CFLAGS)

clean:
	rm -f candymatch
	rm -f core

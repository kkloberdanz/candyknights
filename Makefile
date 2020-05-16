CC=cc
STDFLAG=-std=c99
WARN_FLAGS=-Wall -Wextra -Wpedantic -Werror
LDFLAGS=-lSDL2 -lSDL2_image -lm
CFLAGS=$(OPT) $(WARN_FLAGS)
SANITIZE=-fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined
HEADERS=$(wildcard *.h)
OBJS := $(patsubst %.c,%.o,$(wildcard *.c))

release: OPT=-Os -ffast-math
release: all

debug: OPT=-O0 -ggdb3 -DDEBUG $(SANITIZE)
debug: all

valgrind: OPT=-Og -ggdb3 -DDEBUG
valgrind: OTHER_RUNTIME=valgrind --leak-check=full
valgrind: all

all: candyknights

candyknights: $(OBJS)
	$(CC) -o candyknights $(OBJS) $(LDFLAGS)

main.o: main.c $(HEADERS)
	$(CC) -c main.c $(CFLAGS)

textures.o: textures.c $(HEADERS)
	$(CC) -c textures.c $(CFLAGS)

entity.o: entity.c $(HEADERS)
	$(CC) -c entity.c $(CFLAGS)

controller: controller.c $(HEADERS)
	$(CC) -c controller.c $(CFLAGS)

clean:
	rm -f candyknights
	rm -f core
	rm -f *.o

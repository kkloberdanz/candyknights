CC=cc
STDFLAG=-std=c99
WARN_FLAGS=-Wall -Wextra -Wpedantic -Werror
LDFLAGS=$(OPT) -lSDL2 -lSDL2_image -lm
CFLAGS=$(OPT) $(WARN_FLAGS)
SANITIZE=-fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined
HEADERS=$(wildcard *.h)
OBJS := $(patsubst %.c,%.o,$(wildcard *.c))

.PHONY: release
release: OPT=-Os -ffast-math
release: all

.PHONY: debug
debug: OPT=-O0 -ggdb3 -DDEBUG $(SANITIZE)
debug: all

.PHONY: valgrind
valgrind: OPT=-Og -ggdb3 -DDEBUG
valgrind: all
valgrind:
	valgrind ./candyknights

.PHONY: all
all: candyknights

candyknights: $(OBJS)
	$(CC) -o candyknights $(OBJS) $(LDFLAGS)

main.o: main.c $(HEADERS)
	$(CC) -c main.c $(CFLAGS)

textures.o: textures.c $(HEADERS)
	$(CC) -c textures.c $(CFLAGS)

entity.o: entity.c $(HEADERS)
	$(CC) -c entity.c $(CFLAGS)

controller.o: controller.c $(HEADERS)
	$(CC) -c controller.c $(CFLAGS)

cake.o: cake.c $(HEADERS)
	$(CC) -c cake.c $(CFLAGS)

.PHONY: clean
clean:
	rm -f candyknights
	rm -f core
	rm -f *.o

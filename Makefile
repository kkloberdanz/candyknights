CC=cc
STDFLAG=-std=c99
WARN_FLAGS=-Wall -Wextra -Wpedantic -Werror
LDFLAGS=-lSDL2 -lSDL2_image -lm
CFLAGS=$(OPT) $(WARN_FLAGS)
SANITIZE=-fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined

release: OPT=-Os -ffast-math
release: all

debug: OPT=-O0 -ggdb3 -DDEBUG $(SANITIZE)
debug: all

valgrind: OPT=-Og -ggdb3 -DDEBUG
valgrind: OTHER_RUNTIME=valgrind --leak-check=full
valgrind: all

all: candyknights

candyknights: main.o textures.o entity.o
	$(CC) -o candyknights main.o textures.o entity.o $(LDFLAGS)

main.o: main.c entity.h sprite.h
	$(CC) -c main.c $(CFLAGS)

textures.o: textures.c  textures.h
	$(CC) -c textures.c $(CFLAGS)

entity.o: entity.c entity.h sprite.h textures.h
	$(CC) -c entity.c $(CFLAGS)

clean:
	rm -f candyknights
	rm -f core
	rm -f *.o

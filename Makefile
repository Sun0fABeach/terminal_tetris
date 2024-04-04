CC := gcc
CFLAGS := -W -Wall -Wextra -Wpedantic -std=c2x
LDFLAGS := -lncurses

.PHONY: debug prod clean

debug: CFLAGS += -g3
prod: CFLAGS += -O3
debug prod: tetris

tetris: game.o ui.o input.o
tetris.o: input.h game.h
game.o: game.h ui.h shared.h
ui.o: ui.h shared.h
input.o: input.h

clean:
	rm *.o tetris

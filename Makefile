CC := gcc
CFLAGS := -g3 -W -Wall -Wextra -Wpedantic -std=c2x
LDFLAGS := -lncurses

tetris: game.o ui.o input.o
tetris.o: input.h game.h
game.o: game.h ui.h shared.h
ui.o: ui.h shared.h
input.o: input.h


CC := gcc
CFLAGS := -g3 -W -Wall -Wextra -Wpedantic -std=c2x
LDFLAGS := -lncurses

tetris: ui.o input.o
tetris.o input.o: input.h
tetris.o ui.o: shared.h ui.h


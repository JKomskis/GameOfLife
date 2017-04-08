CC=g++
CFLAGS=-c -Wall -pedantic -std=c++11

all: GameOfLife

GameOfLife: GameOfLife.o Controller.o Board.o Formats.o Util.o pattern.o
	$(CC) GameOfLife.o Controller.o Board.o Formats.o Util.o pattern.o -o GameOfLife -lncurses -lpanel -lform -lmenu

GameOfLife.o: GameOfLife.cpp
	$(CC) $(CFLAGS) GameOfLife.cpp

Controller.o: Controller.cpp Board.o
	$(CC) $(CFLAGS) Controller.cpp

Board.o: Board.cpp Util.o
	$(CC) $(CFLAGS) Board.cpp

pattern.o: Board.cpp
	$(CC) $(CFLAGS) pattern.cpp

Formats.o: Formats.cpp Util.o
	$(CC) $(CFLAGS) Formats.cpp

Util.o:
	$(CC) $(CFLAGS) Util.cpp

clean:
	rm *.o

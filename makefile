CC=g++
CFLAGS=-c -Wall -pedantic -std=c++11

all: GameOfLife

GameOfLife: GameOfLife.o Controller.o Board.o Util.o
	$(CC) GameOfLife.o Controller.o Board.o Util.o -o GameOfLife -lncurses -lpanel -lform -lmenu

GameOfLife.o: GameOfLife.cpp
	$(CC) $(CFLAGS) GameOfLife.cpp

Controller.o: Controller.cpp Board.o
	$(CC) $(CFLAGS) Controller.cpp

Board.o: Board.cpp Util.o
	$(CC) $(CFLAGS) Board.cpp

Util.o:
	$(CC) $(CFLAGS) Util.cpp

clean:
	rm *.o

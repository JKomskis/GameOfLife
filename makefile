CC=g++
CFLAGS=-c -Wall -pedantic -std=c++11

all: GameOfLife

GameOfLife: GameOfLife.o Controller.o Board.o
	$(CC) GameOfLife.o Controller.o Board.o -o GameOfLife -lncurses -lpanel

GameOfLife.o: GameOfLife.cpp
	$(CC) $(CFLAGS) GameOfLife.cpp

Controller.o: Controller.cpp Board.o
	$(CC) $(CFLAGS) Controller.cpp

Board.o: Board.cpp
	$(CC) $(CFLAGS) Board.cpp

clean:
	rm *.o
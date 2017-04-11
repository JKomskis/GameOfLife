set CC=g++
set CFLAGS=-c -Wall -pedantic -std=c++11

%CC% %CFLAGS% Board.cpp
%CC% %CFLAGS% GameOfLife.cpp
%CC% %CFLAGS% Formats.cpp
%CC% %CFLAGS% Util.cpp
%CC% %CFLAGS% Pattern.cpp
%CC% %CFLAGS% Controller.cpp
%CC% %CFLAGS% GameOfLife.cpp

%CC% GameOfLife.o Controller.o Board.o Formats.o Util.o Pattern.o -o GameOfLife.exe -lncurses -lpanelw -lformw -lmenuw

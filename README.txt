              Conway's Game Of Life
What is it?
-----------
Conway’s Game of Life is a cellular automaton
created in 1970 by the mathematician John Conway.
It is a simulation that takes a user’s input to
create an initial configuration, and uses four
simple rules to decide the state of each cell
on the next iteration:
    1. Any live cell with fewer than two live 
       neighbors dies of underpopulation. 
    2. Any live cell with two or three live
       neighbors lives on the next generation. 
    3. Any live cell with greater than three
       neighbors dies of overpopulation. 
    4. Any dead cell with exactly three live
       neighbors become a live cell by reproduction. 
Our project is an implementation of Conway’s Game of
Life in C++. It is based in the terminal and the
ncurses library is used to aid with graphical output.
Furthermore, we will allow users to identify,
manipulate, and save patterns. These features
highlight the immense variety of novel structures
that arise in Game of Life.

Files
-----
Board.cpp        Contains implementation of the board,
                 which manages all the cells and backend
				 logic
Board.h          Header file for the board
Controller.cpp   Contains implementation of the controller,
                 which interfaces between the board module
				 and the ncurses library, acting as the frontend
Controller.h     Header file for the controller
Formats.cpp      Contains implementation of the Formats,
                 which handles all file I/O
Formats.h        Header file for the formats
GameOfLife.cpp   Contains implementation of the GameOfLife
				 module, which contains the main logic loop
				 and controls the execution of the program
make.bat         Batch file to build the project on Windows
makefile         Makefile to build the project on Linux
Pattern.cpp      Contains implementation of the pattern,
				 which inherits from the board and allows
				 for roatation
Pattern.h        Header for the pattern
Util.cpp         Contains implementation of miscellaneous
                 functions used throughout the project
Util.h           Header for the util

Installation
------------
For Windows:
    1. Download MinGW Installation Manager
        a. Go to http://www.mingw.org/
        b. Click download/installer on the left
        c. Follow the instructions to install MinGW
    2. Open the MinGW Installation Manager
    3. Install mingw32-gcc-g++
        a. Check the box next to this and 
           select “Mark for Installation”
        b. In the top left corner, click on
           installation and select “Apply Changes”
    4. Install libncurses
        a. Click on “All Packages” on the left and
           scroll down until you find: mingw32-libncurses
        b. Select both of these and follow the
           instructions in step 3 for installing them
    5. Fix the library paths
        a. Open your File Explorer (Windows + E)
           and navigate to your C drive where 
           MinGW is saved.
        b. Open MinGW/Include/ncursesw
        c. Copy all of the files in ncursesw one 
           into your include folder (basically
           bringing them one directory up)
    6. Build GameOfLife
        a. Open a terminal and browse to your
           GameOfLife folder
        b. Enter "make" into the terminal to build
           the project
        c. Enter "GameOfLife.exe" in the terminal
           to run the program
For Linux:
    1. Install g++ and ncurses
       a. On Ubuntu: open a terminal and enter
          "apt-get intall g++ libncurses-dev"
    2. Build GameOfLife
        a. Open a terminal and browse to your
           GameOfLife folder
        b. Enter "make" into the terminal to build
           the project
        c. Enter "./GameOfLife" in the terminal
           to run the program

Documentation/Usage
-------------------
    At the start of the program the user is greeted by
a main menu. This menu contains all operating modes of
the program and can be accessed at any time by the esc
button. The program can be closed at any time by choosing
the "Exit" option of the menu.
	Load a new board. When this option is selected, a new
board will be created to fit the current terminal size.
The user is then prompted whether or not to enable wrap
around. With wrap around enabled, the program will treat
all edges of the board as wrapping around to their opposite
edge. After selecting yes or no, the user is presented with
the board and the status window. The status window displays
information about the board, including its size, state,
iterations, cell births, cell deaths, and speed.
	Load a saved board. When this option is selected, a prompt
will appear to take a filename. The filename should be in the
"/boards" folder relative to the GameOfLife executable, and
should include its extension. Boards which are larger than the
terminal, nonexistant board, and improerly formatted boards
cannot be loaded. After entering a valid filename, the loaded
board and corresponding status window will be displayed, just
as when "Load a new board" is selected. Loading a saved board
will also load any custom birth and/or survival rules in the
file.
	Load a random board. This option is similar to "Create a
new board", except that after enabling or disabling wrap
around, the program prompts for a percentage of cells to randomly
turn on. The input percentage must be a number between 0 and
100, inclusive. Loading a random board can be cancelled by
entering nothing in the input box and pressing the Enter key.
    Any loaded board will initially be in the paused state. In this
state, the user can user the arrow keys to move the cursor
around the screen. Pressing space will toggle the board cell
at the current cursor location. Lives cells are denoted by an X,
and dead cells are left blank. The user can advance the simulation
forward one iteration by pressing the Enter key. The user can add
a pattern to the board by pressing a. The user is then
prompted to enter a filename. The filename should be the name
of a file in "/patterns" relative to the GameOfLife executable
and should include its extension. The user will be warned if 
the pattern they enter does not exist, is improperly formatted,
or is too large to fit in the current board. The user can cancel
placing a pattern by pressing enter when no text is input in the
filename field. After loading a valid pattern, a red preview of
the pattern will appear on the board. The user can move the
pattern with the arrow keys, rotate the pattern with the ; and
' keys, and place the pattern with the Enter key. To cancel
placing the pattern, press the esc key.
	Load the patten editor. This option prompts the user
for an explicit height and width in which the user can create
a new pattern. The height and width must be greater than 0 and
less than the terminal height and width, respectively. After
entering a valid height and width, a new board of those dimensions
will be created. This board can be edited the same way as any
other board. Saved patterns can then be loaded into any other
board or pattern.
	When any change is made to a board or pattern, the user will
be prompted to save it. If the program is currently manipulating
a board, the file will be saved to the "/boards" folder
relative to the GameOfLife executable. If the program is in
the pattern editor, the file will be saved to the "/patterns"
folder relative to the GameOfLife executable. The chosen filename
must include the .brd file extension at the end of it.

Authors
-------
Erika Bryant
Swathi Chittlur
Katarina Jurczyk
John Kearney
Joseph Komskis
Will Owens
Jonathan Peritz
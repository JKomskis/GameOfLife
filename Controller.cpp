#include "Controller.h"

#define MAIN_MENU_HEIGHT 6
#define MAIN_MENU_WIDTH 40
#define BOARD_HEIGHT termRow * .9
#define BOARD_WIDTH termCol

Controller::Controller()
{
    int maxY = 0, maxX = 0;
    getmaxyx(stdscr, maxY, maxX);
    termRow = maxY;
    termCol = maxX;
    speed = 1000;
    state = "Loading";
    board = new Board(true, BOARD_HEIGHT - 2, BOARD_WIDTH - 2);
    //Create window for the board
    WINDOW *boardWin = newwin(BOARD_HEIGHT, BOARD_WIDTH, 0, 0);
    boardPanel = new_panel(boardWin);
    box(boardWin, 0, 0);
    mvwprintw(boardWin, 1, 1, "+");
    //Create window for the bottom status area
    WINDOW *statusWin = newwin(termRow - BOARD_HEIGHT + 1, BOARD_WIDTH, BOARD_HEIGHT, 0);
    statusPanel = new_panel(statusWin);
    box(statusWin, 0, 0);
    updateStatusWin();
    //Create Main Menu
    WINDOW *mainMenuWin = newwin(MAIN_MENU_HEIGHT, MAIN_MENU_WIDTH,
                            termRow / 2 - MAIN_MENU_HEIGHT / 2, termCol / 2 - MAIN_MENU_WIDTH / 2);
    keypad(mainMenuWin, TRUE);
    mainMenuPanel = new_panel(mainMenuWin);
    box(mainMenuWin, 0, 0);
    printCenter(mainMenuWin, "Conway's Game of Life", 0, 40);
    mvwprintw(mainMenuWin, 1, 2, "Load a new board");
    mvwprintw(mainMenuWin, 2, 2, "Load a saved board");
    mvwprintw(mainMenuWin, 3, 2, "Load a random board");
    mvwprintw(mainMenuWin, 4, 2, "Load the pattern editor");
    mvwprintw(mainMenuWin, 1, 1, ">");
    updateScreen();
}

void Controller::printCenter(WINDOW *win, std::string str, int row, int width)
{
    int col = width / 2 - str.size() / 2;
    mvwprintw(win, row, col, str.c_str());
}

void Controller::updateScreen()
{
    update_panels();
    doupdate();
}

int Controller::getMainMenuChoice()
{
    WINDOW* mainMenuWin = panel_window(mainMenuPanel);
    show_panel(mainMenuPanel);
    update_panels();
    doupdate();
    wchar_t c = 'a';
    int x = 0, y = 0;
    while((c = wgetch(mainMenuWin)) != '\n')
	{
        getyx(mainMenuWin, y, x);
        switch(c)
	    {
            case KEY_DOWN:
                mvwprintw(mainMenuWin, y, --x, " ");
                ++y;
                if(y == MAIN_MENU_HEIGHT-1)
                    y = 1;
                mvwprintw(mainMenuWin, y, x, ">");
                update_panels();
                doupdate();
				break;
			case KEY_UP:
                mvwprintw(mainMenuWin, y, --x, " ");
                --y;
                if(y == 0)
                    y = MAIN_MENU_HEIGHT - 2;
                mvwprintw(mainMenuWin, y, x, ">");
                update_panels();
                doupdate();
				break;
		}
	}
    hide_panel(mainMenuPanel);
    update_panels();
    doupdate();
    getyx(mainMenuWin, y, x);
    return y;
}

void Controller::createNewBoard(bool wrapAround)
{
    delete board;
    board = new Board(wrapAround, BOARD_HEIGHT - 2, BOARD_WIDTH - 2);
}

int Controller::getTermRow()
{
    return termRow;
}

int Controller::getTermCol()
{
    return termCol;
}

int Controller::getSpeed()
{
    return speed;
}

std::string Controller::getState()
{
    return state;
}

void Controller::setState(std::string newState)
{
    state = newState;
    updateStatusWin();
}

void Controller::setSpeed(int newSpeed)
{
    speed = newSpeed;
    updateStatusWin();
}

void Controller::updateStatusWin()
{
    WINDOW* statusWin = panel_window(statusPanel);
    mvwprintw(statusWin, 1, 1, "Board Size:\tStatus:\tSpeed:");
    mvwprintw(statusWin, 3, 1, "%d", board->getHeight());
    wprintw(statusWin, " x ");
    wprintw(statusWin, "%d", board->getWidth());
    wprintw(statusWin, "\t");
    wprintw(statusWin, state.c_str());
    wprintw(statusWin, "\t");
    wprintw(statusWin, "%.2f", speed/1000);
    updateScreen();
}
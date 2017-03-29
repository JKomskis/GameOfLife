#include <ncurses.h>
#include <panel.h>
#include <form.h>
#include <menu.h>
#include <string>
#include "board.h"

class Controller
{
    PANEL *boardPanel;
    PANEL *statusPanel;
    PANEL *mainMenuPanel;
    Board *board = nullptr;
    int termRow, termCol;
    double speed;
    std::string state;

    public:
        Controller();
        void createNewBoard(bool wrapAround);
        void createNewBoard(std::string filename);
        int getMainMenuChoice();
        int getSpeed();
        std::string getState();
        std::string getStringInput();
        int getTermCol();
        int getTermRow();
        void printCenter(WINDOW *win, std::string str, int row, int width);
        void setState(std::string newState);
        void setSpeed(int newSpeed);
        void updateScreen();
        void updateStatusWin();
};
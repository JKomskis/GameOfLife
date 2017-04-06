#include <ncurses.h>
#include <panel.h>
#include <form.h>
#include <menu.h>
#include <string>
#include "Board.h"
#include "Formats.h"

class Controller
{
    PANEL *boardPanel = nullptr;
    PANEL *statusPanel = nullptr;
    PANEL *mainMenuPanel = nullptr;
    Board *board = nullptr;
    int termRow, termCol;

    /*Speed is the frequency of iterations, in iterations/s.
    The system processes this as the period of iterations, or the delay between
    iterations (in ms).*/
    double speed;
    std::string state;

    public:
        Controller();
        void createNewBoard(bool wrapAround);
        void createNewBoard(std::string filename);
        void EditMode();
        void randomizeBoard();
        WINDOW* GetBoardWindow();
        bool GetYesOrNo(std::string dialog);
        int getMainMenuChoice();
        void GetPatternDimensions(int &height, int &width);
        int getSpeed();
        std::string getState();
        std::string getStringInput();
        int getTermCol();
        int getTermRow();
        void printCenter(WINDOW *win, std::string str, int row, int width);
        void setState(std::string newState);
        /*adds the value to the current value of speed. Prevents users from
        decreasing below 1, and exceeding 10000 */
        void setSpeed(int newSpeed);
        void updateScreen();
        void updateStatusWin();
        void printBoard();
        void runIteration();
};

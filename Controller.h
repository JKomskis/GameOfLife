#include <ncurses.h>
#include <panel.h>
#include <form.h>
#include <menu.h>
#include <string>
#include <vector>
#include "Board.h"
#include "Formats.h"
#include "Pattern.h"

enum controlState {menu, running, paused, edit};

class Controller
{
    PANEL *boardPanel = nullptr;
    PANEL *statusPanel = nullptr;
    PANEL *mainMenuPanel = nullptr;
    Board *board = nullptr;
    MENU* mainMenu;
    int termRow, termCol;

    /*Speed is the frequency of iterations, in iterations/s.
    The system processes this as the period of iterations, or the delay between
    iterations (in ms).*/
    int speed;
    controlState state;

    public:
        Controller();
        void createNewBoard(bool wrapAround);
        void createNewBoard(std::string filename);
        bool EditMode();
        void randomizeBoard(double ratio);
        WINDOW* GetBoardWindow();
        bool GetYesOrNo(std::string dialog);
        int getMainMenuChoice();
        void GetPatternDimensions(int &height, int &width);
        int getSpeed();
        controlState getState();
        std::string getStateName();
        double getRatioInput();
        std::string getStringInput(std::string message);
        int getTermCol();
        int getTermRow();
        bool isSaved();
        void printCenter(WINDOW *win, std::string str, int row, int width);
        void SaveCurrent(bool isPattern);
        void setState(controlState newState);
        /*adds the value to the current value of speed. Prevents users from
        decreasing below 1, and exceeding 10000 */
        void setSpeed(int newSpeed);
        void updateScreen();
        void updateStatusWin();
        void printBoard();
        void RenderPattern(std::vector<std::vector<bool>>&);
        void runIteration();
};

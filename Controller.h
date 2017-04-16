#include <ncurses.h>
#include <panel.h>
#include <form.h>
#include <menu.h>
#include <vector>
#include <string>
#include "Board.h"
#include "Formats.h"
#include "Pattern.h"
#include "Util.h"

enum controlState {menu, running, paused, editing, exiting};

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
        void createNewBoard(bool wrapAround, int height, int width);
        void createNewBoard(bool wrapAround);
        void createNewBoard(std::string filename);
        void ConfirmationBox(std::string dialog);
        void EditMode();
        void randomizeBoard(double ratio);
        void KeybindingsBox();
        WINDOW* GetBoardWindow();
        bool GetYesOrNo(std::string dialog);
        int getMainMenuChoice();
        void GetPatternDimensions(int &height, int &width);
        int getSpeed();
        controlState getState();
        std::string getStateName();
        double getRatioInput();
        int getIntInput(std::string message);
        std::string getStringInput(std::string message);
        void getRules();
        int getTermCol();
        int getTermRow();
        bool isSaved();
        void printCenter(WINDOW *win, std::string str, int row, int width);
        void SaveCurrent();
        void setState(controlState newState);
        void setSpeed(int newSpeed);
        void updateScreen();
        void updateStatusWin();
        void printBoard();
        void RenderPattern(std::vector<std::vector<bool>>&);
        void runIteration();
};

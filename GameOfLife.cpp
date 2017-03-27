#include "Controller.h"


int main()
{
    //Setup
    initscr();
    cbreak();
    noecho();
    timeout(-1);
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    Controller *controller = new Controller();
    controller->updateScreen();
    switch( controller->getMainMenuChoice() )
    {
        case 1:
            //Create a new blank board
            controller->createNewBoard(true);
            break;
        case 2:
            //Load a saved board
            //ADD WAY TO GET FILENAME
            //Board board = new Board(filename);
            //controller->setBoard(board);
            break;
        case 3:
            //Load a random board
            break;
        case 4:
            //Load the pattern editor
            break;
    }
    controller->setState("Paused");
    wchar_t input;
    while((input = getch()) != 27)
    {
        if(input == '[')
        {
            controller->setSpeed(controller->getSpeed()-200);
        }
        if(input == ']')
        {
            controller->setSpeed(controller->getSpeed()+200);
        }
        if(input == 'p')
        {
            if(controller->getState() == "Paused")
            {
                controller->setState("Running");
            }
            else
            {
                timeout(-1);
                controller->setState("Paused");
            }
        }
        if(controller->getState() == "Paused")
            continue;
        timeout(controller->getSpeed());
        /*if( mvwinch(boardWin, 1, 1) == '+')
            mvwprintw(boardWin, 1, 1, "-");
        else
            mvwprintw(boardWin, 1, 1, "+");*/
        controller->updateScreen();
    }
    endwin();
    return 0;
}
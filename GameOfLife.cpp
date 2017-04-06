#include "Controller.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>

void MainMenu(Controller *controller)
{
	std::string filename = "";
	bool wrapAround = true;
	switch( controller->getMainMenuChoice() )
	{
		case 0:
			//Create a new blank board
			wrapAround = controller->GetYesOrNo("Would you like to enable wrap around?");
			controller->createNewBoard(wrapAround);
			break;
		case 1:
			//Load a saved board
			filename = controller->getStringInput();
			controller->createNewBoard(filename);
			break;
		case 2:
			//Load a random board
			wrapAround = controller->GetYesOrNo("Would you like to enable wrap around?");
			controller->createNewBoard(wrapAround);
			controller->randomizeBoard();
			break;
		case 3:
			//Load the pattern editor
			break;
	}
	controller->setState("Paused");
	controller->printBoard();
}

int main()
{
    //Setup
	srand(time(0));
    initscr();
    cbreak();
    noecho();
    timeout(-1);
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    Controller *controller = new Controller();
    controller->updateScreen();
	MainMenu(controller);
    wchar_t input = 'a';
    //std::cout << "Printing board." << std::endl;
    controller->updateScreen();
    int count = 0;
    while(controller->getState() == "Paused" || (input = getch()))
    {
        //std::cout << "Iteration count: " << ++count << std::endl;

		if(input == '[')
        {
            controller->setSpeed(-1);
            /*note: continue is used to make sure the simulation doesn't
            stutter after adjusting the speed... it adds a simple pause since
            it skips an iteration*/
            continue;
        }
        else if(input == ']')
        {
            controller->setSpeed(1);
            continue;
        }
        else if(input == 'p' || controller->getState() == "Paused")
        {
            timeout(-1);
            controller->setState("Paused");
			controller->EditMode();
			controller->setState("Running");
        }
		else if(input == 27)
		{
			MainMenu(controller);
		}
        //&& is used for an "advance one iteration" when paused
        if(controller->getState() == "Paused" && input != ' ')
            continue;
        timeout(1.0/controller->getSpeed() * 1000);
        for (int i = 1; (controller->getSpeed() > 1000) && (i < controller->getSpeed() / 1000); i++) {
            controller-> runIteration();
            count++;
        }
        controller->runIteration();
        controller->printBoard();
        controller->updateScreen();
    }
    endwin();
    return 0;
}

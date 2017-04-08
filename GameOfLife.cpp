#include "Controller.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <exception>

void MainMenu(Controller *controller)
{
	std::string filename = "";
	bool wrapAround = true;
	bool isFileValid = false;
	switch( controller->getMainMenuChoice() )
	{
		case 0:
			//Create a new blank board
			wrapAround = controller->GetYesOrNo("Would you like to enable wrap around?");
			controller->createNewBoard(wrapAround);
			break;
		case 1:
			//Load a saved board
			while(!isFileValid)
			{
				filename = controller->getStringInput();
				try
				{
					controller->createNewBoard(filename);
				}catch(char const*)
				{
					continue;
				}
				isFileValid = true;
			}
			break;
		case 2:
			//Load a random board
			wrapAround = controller->GetYesOrNo("Would you like to enable wrap around?");
			controller->createNewBoard(wrapAround);
			controller->randomizeBoard(0.15);
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
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
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
			if(controller->EditMode())
			{
				MainMenu(controller);
			}
			controller->setState("Running");
        }
		else if(input == 27)
		{
			MainMenu(controller);
		}
        //&& is used for an "advance one iteration" when paused
        if(controller->getState() == "Paused")
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

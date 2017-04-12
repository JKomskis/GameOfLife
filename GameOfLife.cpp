#include "Controller.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <exception>

void MainMenu(Controller *controller, bool isSaved)
{
	std::string filename = "";
	bool wrapAround = true;
	bool isFileValid = false;
	switch( controller->getMainMenuChoice() )
	{
		case -1:
			controller->setState(exiting);
		case 0:
			if(!isSaved)
				controller->SaveCurrent(false);
			//Create a new blank board
			wrapAround = controller->GetYesOrNo("Would you like to enable wrap around?");
			controller->createNewBoard(wrapAround);
			controller->setState(paused);
			controller->printBoard();
			break;
		case 1:
			//Load a saved board
			if(!isSaved)
				controller->SaveCurrent(false);
			while(!isFileValid)
			{
				filename = controller->getStringInput("Enter a filename:");
				try
				{
					controller->createNewBoard(filename);
				}catch(char const*)
				{
					continue;
				}
				isFileValid = true;
			}
			controller->setState(paused);
			controller->printBoard();
			break;
		case 2:
		{
			if(!isSaved)
				controller->SaveCurrent(false);
			//Load a random board
			wrapAround = controller->GetYesOrNo("Would you like to enable wrap around?");
			controller->createNewBoard(wrapAround);
			double ratio = controller->getRatioInput();
			controller->randomizeBoard(ratio);
			controller->setState(paused);
			controller->printBoard();
			break;
		}
		case 3:
			if(!isSaved)
				controller->SaveCurrent(false);
			//Load the pattern editor
			controller->createNewBoard(controller->getIntInput("Enter the height of your pattern: "), controller->getIntInput("Enter the width of your pattern: "));
			controller->setState(editing);
			controller->printBoard();
			break;
		case 4:
			if(!isSaved)
				controller->SaveCurrent(false);
			controller->setState(exiting);
	}

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
	MainMenu(controller, true);
    wchar_t input = 'a';
    controller->updateScreen();
	while (controller->getState() != exiting)
	{
		switch (controller->getState())
		{
			case menu:
				MainMenu(controller, controller->isSaved());
				break;
			case running:
				timeout(1.0/controller->getSpeed() * 1000);
				for (int i = 1; (controller->getSpeed() > 1000) && (i < controller->getSpeed() / 1000); i++) {
					controller-> runIteration();
				}
				controller->runIteration();
				controller->printBoard();
				controller->updateScreen();
				input = getch();
				if (input == '[')
					controller->setSpeed(-1);
				else if (input == ']')
					controller->setSpeed(1);
				else if (input == 'p') {
					controller->setState(paused);
					continue;
				}
				else if (input == 27) {
					controller->setState(menu);
				}

				break;
			case paused:
				controller->EditMode();
				break;
			case editing:
				controller->PatternEditor();
				break;
			case exiting:
				break;

		}
	}
    endwin();
    return 0;
}

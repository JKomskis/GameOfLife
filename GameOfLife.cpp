#include "Controller.h"
#include <ctime>
#include <exception>

void MainMenu(Controller *controller)
{
	std::string filename = "";
	bool wrapAround = true;
	bool isFileValid = false;
	switch( controller->getMainMenuChoice() )
	{
		case -1:
			controller->setState(exiting);
			break;
		case 0:
			//Create a new blank board
			wrapAround = controller->GetYesOrNo("Would you like to enable wrap around?");
			controller->createNewBoard(wrapAround);
			controller->setState(paused);
			controller->printBoard();
			break;
		case 1:
			//Load a saved board
			while(!isFileValid)
			{
				filename = controller->getStringInput("Enter a filename:");
				if(filename == "")
					break;
				try
				{
					controller->createNewBoard(filename);
				}catch(char const* message)
				{
					controller->ConfirmationBox(message);
					continue;
				}
				isFileValid = true;
			}
			if(filename == "")
				break;
			controller->setState(paused);
			controller->printBoard();
			break;
		case 2:
		{
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
		{
			//Load the pattern editor
			int height = 0;
			int width = 0;
			controller->GetPatternDimensions(height, width);
			controller->createNewBoard(false, height, width);
			controller->setState(editing);
			controller->printBoard();
			break;
		}
		case 4:
			controller->setState(exiting);
			break;
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
	init_pair(1, 9, COLOR_BLACK);
    keypad(stdscr, TRUE);
    Controller *controller = new Controller();
    controller->updateScreen();
	MainMenu(controller);
    wchar_t input = 'a';
    controller->updateScreen();
	while (controller->getState() != exiting)
	{
		switch (controller->getState())
		{
			case menu:
				MainMenu(controller);
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
				else if(input == 'k')
				{
					controller->KeybindingsBox();
					continue;
				}
				else if (input == 27) {
					if(!controller->isSaved())
						controller->SaveCurrent();
					controller->setState(menu);
				}

				break;
			case paused:
			case editing:
				controller->EditMode();
				break;
			case exiting:
				break;

		}
	}
    endwin();
    return 0;
}

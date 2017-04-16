#include "Controller.h"
#include <ctime>
#include <exception>

void MainMenu(Controller *controller)
{
	switch( controller->getMainMenuChoice() )
	{
		//Create a new board
		case 0:
		{
			bool wrapAround = controller->GetYesOrNo("Would you like to enable wrap around?");
			controller->createNewBoard(wrapAround);
			controller->setState(paused);
			controller->printBoard();
			break;
		}
		//Load a saved board
		case 1:
		{
			std::string filename = "";
			bool isFileValid = false;
			//Loop until we get a valid filename
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
			//If user entered nothing, cancel loading a saved board
			if(filename == "")
				break;
			controller->setState(paused);
			controller->printBoard();
			break;
		}
		//Load a random board
		case 2:
		{
			bool wrapAround = controller->GetYesOrNo("Would you like to enable wrap around?");
			controller->createNewBoard(wrapAround);
			double ratio = controller->getRatioInput();
			//If the user entered nothing, cancel laoding a random board
			if(ratio == -1)
				break;
			controller->randomizeBoard(ratio);
			controller->setState(paused);
			controller->printBoard();
			break;
		}
		//Load the pattern editor
		case 3:
		{
			int height = 0;
			int width = 0;
			controller->GetPatternDimensions(height, width);
			controller->createNewBoard(false, height, width);
			controller->setState(editing);
			controller->printBoard();
			break;
		}
		//Exit
		case -1:
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
	//turn off the keyboard buffer
    cbreak();
    noecho();
    timeout(-1);
	//hide the terminal cursor
    curs_set(FALSE);
	start_color();
	//9 corresponds to bright red
	init_pair(1, 9, COLOR_BLACK);
	//Allows use of the arrow keys
    keypad(stdscr, TRUE);

    Controller *controller = new Controller();
    controller->updateScreen();
	MainMenu(controller);
    wchar_t input = 'a';
    controller->updateScreen();
	//Main control loop
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
				//Simulation controls
				if (input == '[')
					controller->setSpeed(-1);
				else if (input == ']')
					controller->setSpeed(1);
				else if (input == 'p')
					controller->setState(paused);
				else if(input == 'k')
					controller->KeybindingsBox();
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
	//Clean up ncurses
    endwin();
    return 0;
}

#include "Controller.h"
#include <iostream>

#define MAIN_MENU_HEIGHT 8
#define MAIN_MENU_WIDTH 28
#define BOARD_HEIGHT (termRow - STATUS_HEIGHT)
#define BOARD_WIDTH termCol
#define STATUS_HEIGHT 5
#define STATUS_WIDTH termCol

#include <iostream>

Controller::Controller()
{
    int maxY = 0, maxX = 0;
    getmaxyx(stdscr, maxY, maxX);
    termRow = maxY;
    termCol = maxX;
    //Set default speed to 25
    speed = 25;
    //Create the menu and keep it in memory so we can load it faster later
    ITEM **choices = new ITEM*[6];
    choices[0] = new_item("Load a new board", "Load a new board");
    choices[1] = new_item("Load a saved board", "Load a saved board");
    choices[2] = new_item("Load a random board", "Load a random board");
    choices[3] = new_item("Load the pattern editor", "Load the pattern editor");
    choices[4] = new_item("Exit", "Exit");
    choices[5] = NULL;
	//Create the menu
	mainMenu = new_menu(choices);
    menu_opts_off(mainMenu, O_NONCYCLIC);
    menu_opts_off(mainMenu, O_SHOWDESC);
    //create the menu window
    WINDOW *menuWin = newwin(MAIN_MENU_HEIGHT, MAIN_MENU_WIDTH,
                                    termRow / 2 - MAIN_MENU_HEIGHT / 2, termCol / 2 - MAIN_MENU_WIDTH / 2);
    //Allow use of arrow keys
    keypad(menuWin, TRUE);
    mainMenuPanel = new_panel(menuWin);
    set_menu_win(mainMenu, menuWin);
    //Create the menu subwindow
    set_menu_sub(mainMenu, derwin(menuWin, 5, 25, 2, 1));
    //Styling
    set_menu_mark(mainMenu, ">");
    printCenter(menuWin, "Main Menu", 1, 28);
    box(menuWin, 0, 0);
    state = menu;

}

/*Preconditions: win is a properly constructed window object
                str is a properly constructed string object
                row is an integer between 0 and the window height-1, inclusive
                width is the width of the win
Postconditions: prints str at row row in window win
*/
void Controller::printCenter(WINDOW *win, std::string str, int row, int width)
{
    int col = width / 2 - str.size() / 2;
    mvwprintw(win, row, col, str.c_str());
}

/*Postconditions: updates the terminal screen
*/
void Controller::updateScreen()
{
    update_panels();
    doupdate();
}

/*Postconditions: Returns an integer -1 through 4 corresponding to the user's
                    menu choice
*/
int Controller::getMainMenuChoice()
{
	//Show the menu on the screen
	post_menu(mainMenu);
    show_panel(mainMenuPanel);
    WINDOW *mainMenuWin = panel_window(mainMenuPanel);
	wrefresh(mainMenuWin);
    wchar_t ch;
    //Loop until the user presses enter (10) or esc (27)
	while((ch = wgetch(mainMenuWin)) != 10 && ch != 27)
	{       switch(ch)
	        {
				case KEY_DOWN:
					menu_driver(mainMenu, REQ_DOWN_ITEM);
					break;
				case KEY_UP:
					menu_driver(mainMenu, REQ_UP_ITEM);
					break;
				default:
					break;
			}
            wrefresh(mainMenuWin);
	}
    int choice = 0;
    if(ch == 27)
        choice = -1;
    else
        choice = item_index(current_item(mainMenu));
    //hide the menu
    unpost_menu(mainMenu);
    hide_panel(mainMenuPanel);
    updateScreen();
    return choice;
}

/*Preconditions: wrapAround is true if the user wants to enable wrapAround
                and false if the user does not
Postconditions: creates a new board with the max possible height and width
*/
void Controller::createNewBoard(bool wrapAround) {
    createNewBoard(wrapAround, BOARD_HEIGHT, BOARD_WIDTH);
}

/*Preconditions: wrapAround is true if the user wants to enable wrapAround
                and false if the user does not
                height and width are integers
Postconditions: creates a new board. If height or width are <= 0, sets it
                to 1, if height or width are greater than the terminal size,
                sets them to the max possible size
*/
void Controller::createNewBoard(bool wrapAround, int height, int width)
{
    //delete the old board if necessary
    if(board != nullptr){
        delete board;
        board = nullptr;
        wclear(panel_window(boardPanel));
        wclear(panel_window(statusPanel));
    }
    //check the height and width and adjust if necessary
    width = (width > 0) ? width:1;
    width = (width <= BOARD_WIDTH-2) ? width:BOARD_WIDTH-2;
    height = (height > 0) ? height:1;
    height = (height <= BOARD_HEIGHT-2) ? height:BOARD_HEIGHT-2;
    board = new Board(wrapAround, height, width);
    //Create window for the board
    WINDOW *boardWin = newwin(height+2, width+2, (BOARD_HEIGHT-height)/2-1, (BOARD_WIDTH-width)/2-1);
    boardPanel = new_panel(boardWin);
    box(boardWin, 0, 0);
    //Create window for the bottom status area
    WINDOW *statusWin = newwin(STATUS_HEIGHT, STATUS_WIDTH, BOARD_HEIGHT, 0);
    statusPanel = new_panel(statusWin);
    box(statusWin, 0, 0);
    updateStatusWin();
    updateScreen();
}

/*Preconditions: filename is a properly constructed string
Postconditions: Constructs a new board based on filename
                Throws an error if the loaded board is bigger than the
                max obard size
*/
void Controller::createNewBoard(std::string filename)
{
    //delete previous board if necessary
    if(board != nullptr){
        delete board;
        board = nullptr;
        wclear(panel_window(boardPanel));
        wclear(panel_window(statusPanel));
    }
    //May throw an error if the file does not exist
    board = new Board("boards" + separator() + filename);
    int height = board->getHeight();
    int width = board->getWidth();
    //Throw an error if the board is bigger than the max board size
    if(height > BOARD_HEIGHT || width > BOARD_WIDTH)
    {
        throw "Board is too big!";
    }
    // width = (width > 0) ? width:1;
    // width = (width <= BOARD_WIDTH-2) ? width:BOARD_WIDTH-2;
    // height = (height > 0) ? height:1;
    // height = (height <= BOARD_HEIGHT-2) ? height:BOARD_HEIGHT-2;
    //Create a new board window to display the board
    WINDOW *boardWin = newwin(height+2, width+2, (BOARD_HEIGHT-height)/2-1, (BOARD_WIDTH-width)/2-1);
    boardPanel = new_panel(boardWin);
    box(boardWin, 0, 0);
    //Create window for the bottom status area
    WINDOW *statusWin = newwin(STATUS_HEIGHT, STATUS_WIDTH, BOARD_HEIGHT, 0);
    statusPanel = new_panel(statusWin);
    box(statusWin, 0, 0);
    updateStatusWin();
    updateScreen();
}

/*Preconditions: ratio is a double >= 0
Postconditions: Randomly chooses to toggle each cell of the board, based on ratio
*/
void Controller::randomizeBoard(double ratio)
{
    board->randomize(ratio);
}

/*Postconditions: returns terminal height
*/
int Controller::getTermRow()
{
    return termRow;
}

/*Postconditions: returns terminal width
*/
int Controller::getTermCol()
{
    return termCol;
}

/*Postconditions: returns controller speed
*/
int Controller::getSpeed()
{
    return speed;
}

/*Postconditions: returns the current save status of the board
*/
bool Controller::isSaved()
{
    return board->getIsSaved();
}

/*Postconditions: returns controller state
*/
controlState Controller::getState()
{
    return state;
}

/*Postconditions: returns controller state in string form
*/
std::string Controller::getStateName()
{
    switch(state)
    {
        case running:
            return "Running";
        case paused:
            return "Paused";
        case editing:
            return "Editing";
        case menu:
            return "Menu";
        case exiting:
            return "Exiting";
    }
    return "MENU";
}

/*Preconditions: newState is one of the control states delcared in controller.h
Postconditions: sets the controller state to newState
*/
void Controller::setState(controlState newState)
{
    state = newState;
    if(state != exiting)
        updateStatusWin();
}

/*Preconditions: newSpeed is -1 or 1
Postconditions: increases or decreases the controller speed, based on the value
                of newSpeed and the current speed
*/
void Controller::setSpeed(int newSpeed)
{
    /*adds the value to the current value of speed. Prevents users from
    decreasing below 1, and exceeding 10000
    scaling the increment depending on the size of the speed (to allow users
    to scroll at a reasonable rate) */
    if (speed >= 10000)
    {
        newSpeed *= 1000;
    }

    else if (speed >= 10000) {
        newSpeed *= 1000;
    }
    else if (speed >= 1000)
    {
        newSpeed *= 100;
    }
    else if (speed >= 100)
    {
        newSpeed *= 10;
    }
    //make sure the speed is >= 1
    if (speed+newSpeed < 1)
    {
        speed = 1;
        updateStatusWin();
        return;
    }
    //make sure speed does not exceed 100000
    if (speed+newSpeed > 100000)
    {
        speed = 100000;
        updateStatusWin();
        return;
    }
    speed += newSpeed;
    updateStatusWin();
}

/*Postconditions: Updates the status windows, based on the current values of
                height, width, state, iterations, birth, death, and speed
*/
void Controller::updateStatusWin()
{
    WINDOW* statusWin = panel_window(statusPanel);
    werase(statusWin);
    box(statusWin, 0, 0);
    if (state == editing)
        mvwprintw(statusWin, 1, 1, "Board Size:\tStatus:");
    else
        mvwprintw(statusWin, 1, 1, "Board Size:\tStatus:\tIterations:\tBirths:\tDeaths:\tSpeed:\tBirth Rule:\tSurvival Rule:");
    //pad the board size when it is small
    mvwprintw(statusWin, 3, 1, "%-3d", board->getHeight());
    wprintw(statusWin, " x ");
    wprintw(statusWin, "%-3d", board->getWidth());
    wprintw(statusWin, "\t");
    wprintw(statusWin, getStateName().c_str());
    wprintw(statusWin, "\t");
    if (state != editing)
    {
        wprintw(statusWin, "%d", board->getIterations());
        wprintw(statusWin, "\t\t");
        wprintw(statusWin, "%d", board->getBirths());
        wprintw(statusWin, "\t");
        wprintw(statusWin, "%d", board->getDeaths());
        wprintw(statusWin, "\t");

        /*speed variable is in iterations/second... make sure it is presented that
        way to the user (i.e. don't divide by 1000 or anything) */
        wprintw(statusWin, "%d", speed);
        
        wprintw(statusWin, "\t");
        wprintw(statusWin, "%-9s", board->getBirthRule().c_str());
        wprintw(statusWin, "\t");
        wprintw(statusWin, "%-9s", board->getSurvivalRule().c_str());
    }
    mvwprintw(statusWin, 2, STATUS_WIDTH-15, "K: Keybindings");
    updateScreen();
}

/*Preconditions: message is a string object
Postconditions: Prompts the user with message, and gets string input from the user
*/
std::string Controller::getStringInput(std::string message)
{
    //dispaly the cursor
    curs_set(TRUE);
    //create the input fields and configure them
    FIELD *field[2];
	int rows, cols;
	field[0] = new_field(1, termCol / 4, 1, 1, 0, 0);
	field[1] = NULL;
	set_field_back(field[0], A_UNDERLINE);
	field_opts_off(field[0], O_AUTOSKIP);
    field_opts_off(field[0], O_STATIC);
    set_max_field(field[0], 30);
    //Create the form
	FORM *form = new_form(field);
	scale_form(form, &rows, &cols);
    WINDOW *formWin = newwin(rows + 4, cols + 4, termRow / 2 - (rows + 4) / 2, termCol / 2 - termCol / 8);
    PANEL *formPanel = new_panel(formWin);
    keypad(formWin, TRUE);
    set_form_win(form, formWin);
    set_form_sub(form, derwin(formWin, rows, cols, 2, 2));
    box(formWin, 0, 0);
    //Display the message to the user
    printCenter(formWin, message, 1, cols);
    //Dispaly the form
	post_form(form);
    show_panel(formPanel);
	updateScreen();
    wchar_t ch;
	//Loop until the user presses enter (10)
	while((ch = wgetch(formWin)) != 10)
	{
        switch(ch)
		{
            //Move cursor position left
            case KEY_LEFT:
			    form_driver(form, REQ_PREV_CHAR);
			    break;
            //Move cursor position right
			case KEY_RIGHT:
			    form_driver(form, REQ_NEXT_CHAR);
			    break;
            //Delete the character directly before the current cursor position
            case '\b':
            case KEY_BACKSPACE:
                form_driver(form, REQ_PREV_CHAR);
                form_driver(form, REQ_DEL_CHAR);
                break;
            //Delete the character direcly at the current cursor position
            case KEY_DC:
                form_driver(form, REQ_DEL_CHAR);
                break;
            //Add the character to the input field
			default:
				form_driver(form, ch);
				break;
		}
	}
    //Request validation so we can safely get the field buffer
    form_driver(form, REQ_VALIDATION);
    std::string filename = field_buffer(field[0], 0);
    //Remove any extra spaces on the end
    while(filename.back() == ' ')
		filename.pop_back();
    //Hide cursor and delete the form
    curs_set(FALSE);
    unpost_form(form);
	free_form(form);
	free_field(field[0]);
    hide_panel(formPanel);
    delwin(formWin);
    del_panel(formPanel);
    updateScreen();
    return filename;
}

void Controller::getRules()
{
    //Show the cursor
    curs_set(TRUE);
    //Create the fields and configure them
    FIELD *field[5];
	int rows, cols;
	field[0] = new_field(1, 20, 2, 0, 0, 0);
    field[1] = new_field(1, 10, 2, 22, 0, 0);
    field[2] = new_field(1, 20, 3, 0, 0, 0);
    field[3] = new_field(1, 10, 3, 22, 0, 0);
	field[4] = NULL;
    field_opts_off(field[0], O_ACTIVE);
    set_field_buffer(field[0], 0, "Birth Rule: ");
	set_field_back(field[1], A_UNDERLINE);
	field_opts_off(field[1], O_AUTOSKIP);
    set_field_type(field[1], TYPE_INTEGER, 0, 1, 123456789);
    field_opts_off(field[2], O_ACTIVE);
    set_field_buffer(field[2], 0, "Survival Rule: ");
    set_field_back(field[3], A_UNDERLINE);
	field_opts_off(field[3], O_AUTOSKIP);
    set_field_type(field[3], TYPE_INTEGER, 0, 1, 123456789);
    //Create the form
    FORM *form = new_form(field);
    scale_form(form, &rows, &cols);
    WINDOW *formWin = newwin(rows + 4, cols + 4, termRow / 2 - (rows + 4) / 2, termCol / 2 - cols / 2);
    PANEL *formPanel = new_panel(formWin);
    keypad(formWin, TRUE);
    set_form_win(form, formWin);
    WINDOW* subFormWin = derwin(formWin, rows, cols, 2, 2);
    set_form_sub(form, subFormWin);
    box(formWin, 0, 0);
    printCenter(formWin, "Enter rules:", 1, cols + 4);
    //display the form
    post_form(form);
    show_panel(formPanel);
    updateScreen();
    set<int> birthTemp, survivalTemp;
    bool birthSelected = true;
    wchar_t ch;
    //Loop until the user enters valid values for both fields and presses enter
    while((ch = wgetch(formWin)))
    {
        switch(ch)
        {
            //Switch fields
            case KEY_UP:
            case KEY_DOWN:
            case '\t':
                form_driver(form, REQ_NEXT_FIELD );
                // clear form & rule set when changing to new rule
                birthSelected = birthSelected ? false : true;
                if(birthSelected)
                {
					set_field_buffer(field[1], 0, "");
					birthTemp.clear();
				}
				else
                {
					set_field_buffer(field[3], 0, "");
					survivalTemp.clear();
				}
                break;
            case KEY_LEFT:
				form_driver(form, REQ_PREV_CHAR);
				break;
            case KEY_RIGHT:
				form_driver(form, REQ_NEXT_CHAR);
				break;
            //Delete the character before the cursor
            case '\b':
            case KEY_BACKSPACE:
                form_driver(form, REQ_PREV_CHAR);
                form_driver(form, REQ_DEL_CHAR);
                break;
            //Delete the character at the cursor
            case KEY_DC:
                form_driver(form, REQ_DEL_CHAR);
                break;
            //Check the values of the fields
            //If one is not filled, use the enter key to switch fields
            case 10:
                form_driver(form, REQ_VALIDATION);
                if( birthTemp.size() == 0 || survivalTemp.size() == 0 )
                {
                    form_driver(form, REQ_NEXT_FIELD);
                    break;
                }
                //set the new rules
                board->setBirthRule(birthTemp);
                board->setSurvivalRule(survivalTemp);
                //delete the form
                curs_set(FALSE);
                unpost_form(form);
                free_form(form);
                for(int i = 0; i < 4; ++i)
                {
                    free_field(field[i]);
                }
                hide_panel(formPanel);
                delwin(formWin);
                del_panel(formPanel);
                updateStatusWin();
                updateScreen();
                return;
            case 27:
				return;
            // skip 0 entry
			case '0':
				break;
            //Add the character to the field
            default:
				bool added = false;
				int value = atoi((char*)&ch);
				if(birthSelected)
				{
					if(birthTemp.find(value) == birthTemp.end())
					{
						birthTemp.insert(value);
						added = true;
					}
				}
				else if(survivalTemp.find(value) == survivalTemp.end())
				{
					survivalTemp.insert(value);
					added = true;
				}
				if(added)
					form_driver(form, ch);
                break;
        }
    }
}

/*Postconditions: displays the board in the board window
*/
void Controller::printBoard()
{
    WINDOW *win = panel_window(boardPanel);
    int x = 0, y = 0;
    //save current cursor coordinates
    getyx(win, y, x);
    wmove(win, 1, 1);
    std::vector<std::vector<bool>> matrix = board->getMatrix();
    for (int r = 0; r < board->getHeight(); r++)
    {
        for (int c=0; c < board->getWidth(); c++)
        {
            if(matrix[r][c])
                waddch(win, 'X');
            else
                waddch(win, ' ');

        }
        //move to the next row
        wmove(win, r+2, 1);
    }
    show_panel(boardPanel);
    updateScreen();
    //move cursor back to its original position
    wmove(win, y, x);
}

/*Postconditions: runs an iteration on the board and updates the status window
*/
void Controller::runIteration()
{
    board->runIteration();
    updateStatusWin();
}

/*Preconditions: dialog is a string object
Postconditions: diaplays dialog to the user, and gets a yes or no answer from the user
*/
bool Controller::GetYesOrNo(std::string dialog)
{
    //create the window based on dialog length
    WINDOW *dialogWin = newwin(5, dialog.size() + 4, termRow / 2 - 3, termCol / 2 - (dialog.size() + 4) / 2);
    keypad(dialogWin, TRUE);
    PANEL *dialogPanel = new_panel(dialogWin);
    box(dialogWin, 0, 0);
    mvwprintw(dialogWin, 1, 2, dialog.c_str());
    wattron(dialogWin, A_STANDOUT);
    mvwprintw(dialogWin, 3, (dialog.size() + 4) / 4 - 1, "<Yes>");
    wattroff(dialogWin, A_STANDOUT);
    mvwprintw(dialogWin, 3, 3 * (dialog.size() + 4) / 4 - 1, "<No>");
    show_panel(dialogPanel);
    updateScreen();
    bool yesSelected = true;
    wchar_t ch;
    //loop until the user presses enter (10)
	while((ch = wgetch(dialogWin)) != 10)
	{
        switch(ch)
	    {
            //Toggles the highlighting of the options and toggles yesSelected
            case KEY_LEFT:
			case KEY_RIGHT:
                if(yesSelected)
                {
                    wattroff(dialogWin, A_STANDOUT);
                    mvwprintw(dialogWin, 3, (dialog.size() + 4) / 4 - 1, "<Yes>");
                    wattron(dialogWin, A_STANDOUT);
                    mvwprintw(dialogWin, 3, 3 * (dialog.size() + 4) / 4 - 1, "<No>");
                }
                else
                {
                    wattron(dialogWin, A_STANDOUT);
                    mvwprintw(dialogWin, 3, (dialog.size() + 4) / 4 - 1, "<Yes>");
                    wattroff(dialogWin, A_STANDOUT);
                    mvwprintw(dialogWin, 3, 3 * (dialog.size() + 4) / 4 - 1, "<No>");
                }
                yesSelected = !yesSelected;
                break;
		}
        wrefresh(dialogWin);
	}
    //delete the window
    curs_set(FALSE);
    hide_panel(dialogPanel);
    delwin(dialogWin);
    del_panel(dialogPanel);
    updateScreen();
    return yesSelected;
}

/*Preconditions: dialog is a string
Postconditions: displays dialog to the user, and wait for a keypress
*/
void Controller::ConfirmationBox(std::string dialog)
{
    ///create the window based on dialog length
    WINDOW *dialogWin = newwin(5, dialog.size() + 4, termRow / 2 - 3, termCol / 2 - (dialog.size() + 4) / 2);
    keypad(dialogWin, TRUE);
    PANEL *dialogPanel = new_panel(dialogWin);
    box(dialogWin, 0, 0);
    mvwprintw(dialogWin, 1, 2, dialog.c_str());
    wattron(dialogWin, A_STANDOUT);
    mvwprintw(dialogWin, 3, (dialog.size() + 4) / 2 - 2, "<OK>");
    show_panel(dialogPanel);
    updateScreen();
    //wait for a keypress
    wgetch(dialogWin);
    //delete the window
    curs_set(FALSE);
    hide_panel(dialogPanel);
    delwin(dialogWin);
    del_panel(dialogPanel);
    updateScreen();
}

/*Postconditions: displays the keybindings to the user
*/
void Controller::KeybindingsBox()
{
    //create the window and print the keybindings
    WINDOW *dialogWin = newwin(15, 38, termRow / 2 - 7, termCol / 2 - 19);
    keypad(dialogWin, TRUE);
    PANEL *dialogPanel = new_panel(dialogWin);
    box(dialogWin, 0, 0);
    mvwprintw(dialogWin, 1, 1, "p : pause/resume");
    mvwprintw(dialogWin, 2, 1, "[ : decrease speed");
    mvwprintw(dialogWin, 3, 1, "] : increase speed");
    mvwprintw(dialogWin, 4, 1, "Arrow Keys : move cursor");
    mvwprintw(dialogWin, 5, 1, "a : add pattern");
    mvwprintw(dialogWin, 6, 1, "; : rotate pattern counterclockwise");
    mvwprintw(dialogWin, 7, 1, "' : rotate pattern clockwise");
    mvwprintw(dialogWin, 8, 1, "r : edit rules");
    show_panel(dialogPanel);
    updateScreen();
    //wait for a keypress
    wgetch(dialogWin);
    //delete the window
    curs_set(FALSE);
    hide_panel(dialogPanel);
    delwin(dialogWin);
    del_panel(dialogPanel);
    updateScreen();
}

/*Preconditions: height and width are references to integers
Postconditions: sets height and width equal to the user input values
*/
void Controller::GetPatternDimensions(int &height, int &width)
{
    //Show the cursor
    curs_set(TRUE);
    //Create the fields and configure them
    FIELD *field[5];
	int rows, cols;
	field[0] = new_field(1, 8, 2, 0, 0, 0);
    field[1] = new_field(1, 5, 2, 8, 0, 0);
    field[2] = new_field(1, 7, 2, 15, 0, 0);
    field[3] = new_field(1, 5, 2, 22, 0, 0);
	field[4] = NULL;
    field_opts_off(field[0], O_ACTIVE);
    set_field_buffer(field[0], 0, "Height: ");
	set_field_back(field[1], A_UNDERLINE);
	field_opts_off(field[1], O_AUTOSKIP);
    set_field_type(field[1], TYPE_INTEGER, 0, 1, BOARD_HEIGHT);
    field_opts_off(field[2], O_ACTIVE);
    set_field_buffer(field[2], 0, "Width: ");
    set_field_back(field[3], A_UNDERLINE);
	field_opts_off(field[3], O_AUTOSKIP);
    set_field_type(field[3], TYPE_INTEGER, 0, 1, BOARD_WIDTH);
    //Create the form
    FORM *form = new_form(field);
    scale_form(form, &rows, &cols);
    WINDOW *formWin = newwin(rows + 4, cols + 4, termRow / 2 - (rows + 4) / 2, termCol / 2 - cols / 2);
    PANEL *formPanel = new_panel(formWin);
    keypad(formWin, TRUE);
    set_form_win(form, formWin);
    WINDOW* subFormWin = derwin(formWin, rows, cols, 2, 2);
    set_form_sub(form, subFormWin);
    box(formWin, 0, 0);
    printCenter(formWin, "Enter pattern dimensions:", 1, cols + 4);
    //display the form
    post_form(form);
    show_panel(formPanel);
    updateScreen();
    int heightInput = 0, widthInput = 0;
    wchar_t ch;
    //Loop until the user enters valid values for both fields and presses enter
    while((ch = wgetch(formWin)))
    {
        switch(ch)
        {
            //Switch fields
            case KEY_LEFT:
            case KEY_RIGHT:
            case '\t':
                form_driver(form, REQ_NEXT_FIELD);
                break;
            //Delete the character before the cursor
            case '\b':
            case KEY_BACKSPACE:
                form_driver(form, REQ_PREV_CHAR);
                form_driver(form, REQ_DEL_CHAR);
                break;
            //Delete the character at the cursor
            case KEY_DC:
                form_driver(form, REQ_DEL_CHAR);
                break;
            //Check the values of the fields
            //If one is not filled, use the enter key to switch fields
            case 10:
                form_driver(form, REQ_VALIDATION);
                heightInput = atoi( field_buffer(field[1], 0) );
                widthInput = atoi( field_buffer(field[3], 0) );
                if( (heightInput == 0) ^ (widthInput == 0) )
                {
                    form_driver(form, REQ_NEXT_FIELD);
                    break;
                }
                height = heightInput;
                width = widthInput;
                //delete the form
                curs_set(FALSE);
                unpost_form(form);
                free_form(form);
                for(int i = 0; i < 4; ++i)
                {
                    free_field(field[i]);
                }
                hide_panel(formPanel);
                delwin(formWin);
                del_panel(formPanel);
                updateScreen();
                return;

            //Add the character to the field
            default:
                form_driver(form, ch);
                break;
        }
    }
}

/*Postconditions: gets a ratio input from the user
*/
double Controller::getRatioInput()
{
    //show the cursor
    curs_set(TRUE);
    //create the fields and configure them
    FIELD *field[2];
	int rows, cols;
	field[0] = new_field(1, termCol / 4, 1, 1, 0, 0);
	field[1] = NULL;
	set_field_back(field[0], A_UNDERLINE);
	field_opts_off(field[0], O_AUTOSKIP);
    field_opts_off(field[0], O_STATIC);
    set_field_type(field[0], TYPE_INTEGER, 10, 0, 100);
    set_max_field(field[0], 10);
    //create the form
	FORM *form = new_form(field);
	scale_form(form, &rows, &cols);
    WINDOW *formWin = newwin(rows + 6, cols + 4, termRow / 2 - (rows + 6) / 2, termCol / 2 - termCol / 8);
    PANEL *formPanel = new_panel(formWin);
    keypad(formWin, TRUE);
    set_form_win(form, formWin);
    set_form_sub(form, derwin(formWin, rows, cols, 3, 2));
    box(formWin, 0, 0);
    printCenter(formWin, "Enter a percentage of", 1, cols);
    printCenter(formWin, "cells to turn live:", 2, cols);
	post_form(form);
    show_panel(formPanel);
	updateScreen();
    wchar_t ch;
	/* Loop through to get user requests */
    bool isValid = false;
    double ratio = 0;
    //Loop until the user gives a number between 0 and 100, inclusive
	while (!isValid)
	{
        ch = wgetch(formWin);
        switch(ch)
		{
            //move the cursor left
            case KEY_LEFT:
			    form_driver(form, REQ_PREV_CHAR);
			    break;
            //move the cursor right
			case KEY_RIGHT:
			    form_driver(form, REQ_NEXT_CHAR);
			    break;
            //delete the character before the cursor position
            case '\b':
            case KEY_BACKSPACE:
                form_driver(form, REQ_PREV_CHAR);
                form_driver(form, REQ_DEL_CHAR);
                break;
            //delete the character at the cursor position
            case KEY_DC:
                form_driver(form, REQ_DEL_CHAR);
                break;
            //check the current field value
            case '\n':
                form_driver(form, REQ_VALIDATION);
                ratio = atof(field_buffer(field[0], 0));
                if(ratio >= 0 && ratio <= 100)
                    isValid = true;
                break;
            //add the character to the field
			default:
				form_driver(form, ch);
				break;
		}
	}
    //get the ratio from the field
    form_driver(form, REQ_VALIDATION);
    std::string buffer = field_buffer(field[0], 0);
    while(buffer.back() == ' ')
        buffer.pop_back();
    if(buffer == "")
        ratio = -1;
    else
        ratio = atof(buffer.c_str())/100;
    //delete the form
    curs_set(FALSE);
    unpost_form(form);
	free_form(form);
	free_field(field[0]);
    hide_panel(formPanel);
    delwin(formWin);
    del_panel(formPanel);
    updateScreen();
    return ratio;
}

/*A sub control loop, allows the user the manually toggle cells and add patterns
*/
void Controller::EditMode()
{
    WINDOW* boardWin = panel_window(boardPanel);
    keypad(boardWin, TRUE);
    int maxX = 0, maxY = 0;
    getmaxyx(boardWin, maxY, maxX);
    mvwaddch(boardWin, maxY / 2, maxX / 2, winch(boardWin)|A_STANDOUT);
    wmove(boardWin, maxY / 2, maxX / 2);
	int x = 0, y = 0;
    //loop until we are no longer paused or editing
    while(getState() == paused || getState() == editing)
    {
        updateScreen();
        wchar_t input = getch();
        //erase the current cursor
        if( input == KEY_UP || input == KEY_DOWN || input == KEY_LEFT || input == KEY_RIGHT || input == ' ')
        {
            getyx(boardWin, y, x);
            waddch(boardWin, char( winch(boardWin) ));
            wmove(boardWin, y, x);
        }
        std::string filename = "";
        bool isFileValid = false;
        Pattern *pattern = nullptr;
        std::string layout = "";
        std::vector<std::vector<bool>> matrix;
        switch(input)
        {
            //move the cursor up
            case KEY_UP:
                if(y == 1)
                    wmove(boardWin, maxY - 2, x);
                else
                    wmove(boardWin, y-1, x);
                getyx(boardWin, y, x);
                waddch(boardWin, winch(boardWin)|A_STANDOUT);
                wmove(boardWin, y, x);
                break;
            //move the cursor down
            case KEY_DOWN:
                if(y == (maxY - 2))
                    wmove(boardWin, 1, x);
                else
                    wmove(boardWin, y+1, x);
                getyx(boardWin, y, x);
                waddch(boardWin, winch(boardWin)|A_STANDOUT);
                wmove(boardWin, y, x);
                break;
                //move the cursor left
            case KEY_LEFT:
                if(x == 1)
                    wmove(boardWin, y, maxX - 2);
                else
                    wmove(boardWin, y, x-1);
                getyx(boardWin, y, x);
                waddch(boardWin, winch(boardWin)|A_STANDOUT);
                wmove(boardWin, y, x);
                break;
            //move the cursor right
            case KEY_RIGHT:
                if(x == maxX - 2)
                    wmove(boardWin, y, 1);
                else
                    wmove(boardWin, y, x+1);
                getyx(boardWin, y, x);
                waddch(boardWin, winch(boardWin)|A_STANDOUT);
                wmove(boardWin, y, x);
                break;
            //toggle the cell at the current cursor location
            case ' ':
                board->toggle(y-1, x-1);
                printBoard();
                wmove(boardWin, y, x);
                waddch(boardWin, winch(boardWin)|A_STANDOUT);
                wmove(boardWin, y, x);
                break;
            //decrease the speed
            case '[':
                if (state != editing)
                    setSpeed(-1);
                break;
            //increase the speed
            case ']':
                if (state != editing)
                    setSpeed(1);
                break;
            //exit edit mode
            case 'p':
                if (state != editing)
                    setState(running);
                break;
            //get birth/survival rules
            case 'r':
                if (state != editing)
                    getRules();
                break;
            //display the keybindings box
            case 'k':
                KeybindingsBox();
                break;
            //add a pattern
            case 'a':
                getyx(boardWin, y, x);
                //loop until the filename is valid or until they enter nothing
                while(!isFileValid)
                {
                    filename = getStringInput("Enter a filename:");
                    if(filename == "")
                        break;
                    try
                    {
                        pattern = new Pattern("patterns" + separator() + filename);
                        if(pattern->getHeight() > board->getHeight() || pattern->getWidth() > board->getWidth())
                        {
                            throw "Pattern too big!";
                        }
                    }catch(char const* message)
                    {
                        ConfirmationBox(message);
                        continue;
                    }
                    isFileValid = true;
                }
                if(filename == "")
                    break;
                matrix = pattern->getMatrix();
                RenderPattern(matrix);
                do
                {
                    input = wgetch(boardWin);
                    getyx(boardWin, y, x);
                    switch(input)
                    {
                        //move the pattern up
                        case KEY_UP:
                            if(y == 1)
                                wmove(boardWin, maxY - 2, x);
                            else
                                wmove(boardWin, y-1, x);
                            RenderPattern(matrix);
                            break;
                        //move the pattern down
                        case KEY_DOWN:
                            if(y == (maxY - 2))
                                wmove(boardWin, 1, x);
                            else
                                wmove(boardWin, y+1, x);
                            RenderPattern(matrix);
                            break;
                        //move the pattern left
                        case KEY_LEFT:
                            if(x == 1)
                                wmove(boardWin, y, maxX - 2);
                            else
                                wmove(boardWin, y, x-1);
                            RenderPattern(matrix);
                            break;
                        //move the pattern right
                        case KEY_RIGHT:
                            if(x == maxX - 2)
                                wmove(boardWin, y, 1);
                            else
                                wmove(boardWin, y, x+1);
                            RenderPattern(matrix);
                            break;
                        //rotate the pattern clockwise
                        case '\'':
                            pattern->Rotate();
                            matrix = pattern->getMatrix();
                            RenderPattern(matrix);
                            break;
                        //rotate the pattern counter clockwise
                        case ';':
                            pattern->Rotate();
                            pattern->Rotate();
                            pattern->Rotate();
                            matrix = pattern->getMatrix();
                            RenderPattern(matrix);
                            break;
                        //dispaly the keybindings box
                        case 'k':
                            KeybindingsBox();
                            break;
                        //place the pattern
                        case 10:
                            board->addPattern(pattern->getMatrix(), y, x);
                            break;
                    }
                } while(input != 10 && input != 27);
                werase(boardWin);
                box(boardWin, 0, 0);
                printBoard();
                wmove(boardWin, y, x);
                break;
            //run a single iteration
            case 10:
                runIteration();
                printBoard();
                break;
            //Exit edit mode
            case 27:
                if(!isSaved())
                    SaveCurrent();
                setState(menu);
                break;
        }
	}
}

/*Preconditions: matrix is a vector of vectors of bools
Postconditions: renders matrix on top of the board
*/
void Controller::RenderPattern(std::vector<std::vector<bool>>& matrix)
{
    WINDOW *boardWin = panel_window(boardPanel);
    int x = 0, y = 0;
    getyx(boardWin, y, x);
    printBoard();
    wattron(boardWin, COLOR_PAIR(1));
    for(size_t i = 0; i < matrix.size(); ++i)
    {
        for(size_t j = 0; j < matrix[0].size(); ++j)
        {
            mvwprintw(boardWin, (y + i) % board->getMatrix().size() + 1, (x + j) % board->getMatrix()[0].size() + 1, (matrix[i][j]) ? "X" : "");
        }
    }
    wattroff(boardWin, COLOR_PAIR(1));
    wmove(boardWin, y, x);
}

/*Postconditions: Saves the board if the users selects yes
                does nothing otherwise
*/
void Controller::SaveCurrent()
{
    bool shouldSave = GetYesOrNo("Would you like to save?");
    if(shouldSave)
    {
        std::string filename = getStringInput("Enter a filename:");
        //If the state is editing, then the program is in the pattern editor
        //so the file should be saved to the pattern folder
        if(state == editing)
        {
            filename = "patterns" + separator() + filename;
        }
        //Otherwise, save it to the board folder
        else
        {
            filename = "boards" + separator() + filename;
        }
        board->saveState(filename);
    }
}

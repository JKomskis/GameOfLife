#include "Controller.h"

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
    speed = 25;
    //state = "Loading";
    ITEM **choices = new ITEM*[6];
    choices[0] = new_item("Load a new board", "Load a new board");
    choices[1] = new_item("Load a saved board", "Load a saved board");
    choices[2] = new_item("Load a random board", "Load a random board");
    choices[3] = new_item("Load the pattern editor", "Load the pattern editor");
    choices[4] = new_item("Exit", "Exit");
    choices[5] = NULL;
	/* Crate menu */
	mainMenu = new_menu(choices);
    menu_opts_off(mainMenu, O_NONCYCLIC);
    menu_opts_off(mainMenu, O_SHOWDESC);

	/* Create the window to be associated with the menu */
    WINDOW *my_menu_win = newwin(MAIN_MENU_HEIGHT, MAIN_MENU_WIDTH,
                                    termRow / 2 - MAIN_MENU_HEIGHT / 2, termCol / 2 - MAIN_MENU_WIDTH / 2);
    keypad(my_menu_win, TRUE);
    mainMenuPanel = new_panel(my_menu_win);

	/* Set main window and sub window */
    set_menu_win(mainMenu, my_menu_win);
    set_menu_sub(mainMenu, derwin(my_menu_win, 5, 25, 2, 1));

	/* Set menu mark to the string " * " */
    set_menu_mark(mainMenu, ">");
    printCenter(my_menu_win, "Main Menu", 1, 28);
	/* Print a border around the main window and print a title */
    box(my_menu_win, 0, 0);
	//print_in_middle(my_menu_win, 1, 0, 40, "My Menu", COLOR_PAIR(1));
	//refresh();
    state = menu;

}

void Controller::printCenter(WINDOW *win, std::string str, int row, int width)
{
    int col = width / 2 - str.size() / 2;
    mvwprintw(win, row, col, str.c_str());
}

void Controller::updateScreen()
{
    update_panels();
    doupdate();
}

int Controller::getMainMenuChoice()
{
	/* Post the menu */
	post_menu(mainMenu);
    show_panel(mainMenuPanel);
    WINDOW *mainMenuWin = panel_window(mainMenuPanel);
	wrefresh(mainMenuWin);
    wchar_t ch;
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
	/* Unpost and free all the memory taken up */
    else
        choice = item_index(current_item(mainMenu));
    unpost_menu(mainMenu);
    hide_panel(mainMenuPanel);
    updateScreen();
    return choice;
}

void Controller::createNewBoard(bool wrapAround) {
    createNewBoard(wrapAround, BOARD_HEIGHT, BOARD_WIDTH);
}

void Controller::createNewBoard(bool wrapAround, int height, int width)
{
    delete board;
    wclear(panel_window(boardPanel));
    wclear(panel_window(statusPanel));
    /*delwin(panel_window(boardPanel));
    del_panel(boardPanel);
    delwin(panel_window(statusPanel));
    del_panel(statusPanel);*/

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

void Controller::createNewBoard(std::string filename)
{
    delete board;
    wclear(panel_window(boardPanel));
    wclear(panel_window(statusPanel));
    board = new Board(filename);
    //while(true){};
    int height = board->getHeight();
    int width = board->getWidth();
    //TODO: test loading with files larger than terminal.
    width = (width > 0) ? width:1;
    width = (width <= BOARD_WIDTH-2) ? width:BOARD_WIDTH-2;
    height = (height > 0) ? height:1;
    height = (height <= BOARD_HEIGHT-2) ? height:BOARD_HEIGHT-2;
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

void Controller::randomizeBoard(double ratio)
{
    board->randomize(ratio);
}

int Controller::getTermRow()
{
    return termRow;
}

int Controller::getTermCol()
{
    return termCol;
}

int Controller::getSpeed()
{
    return speed;
}

controlState Controller::getState()
{
    return state;
}

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

void Controller::setState(controlState newState)
{
    state = newState;
    updateStatusWin();
}

void Controller::setSpeed(int newSpeed)
{
    /*scaling the increment depending on the size of the speed (to allow users
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
    if (speed+newSpeed < 1)
    {
        speed = 1;
        updateStatusWin();
        return;
    }
    /*speed maximum is 1000 iterations/s equating to a delay of 1 ms
    testing on my toaster laptop, the differences above this point appear
    imperceptible, likely because the time it takes to display all of this
    becomes significant*/
    if (speed+newSpeed > 100000)
    {
        speed = 100000;
        updateStatusWin();
        return;
    }
    speed += newSpeed;
    updateStatusWin();
}

void Controller::updateStatusWin()
{
    WINDOW* statusWin = panel_window(statusPanel);
    werase(statusWin);
    box(statusWin, 0, 0);
    if (state == editing)
        mvwprintw(statusWin, 1, 1, "Board Size:\tStatus:");
    else
        mvwprintw(statusWin, 1, 1, "Board Size:\tStatus:\tIterations:\tBirths:\tDeaths:\tSpeed:");
    mvwprintw(statusWin, 3, 1, "%02d", board->getHeight());
    wprintw(statusWin, " x ");
    wprintw(statusWin, "%d", board->getWidth());
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
    }
    updateScreen();
}

std::string Controller::getStringInput(std::string message)
{
    curs_set(TRUE);
    FIELD *field[2];
	int rows, cols;
	field[0] = new_field(1, termCol / 4, 1, 1, 0, 0);
	field[1] = NULL;
	set_field_back(field[0], A_UNDERLINE);
	field_opts_off(field[0], O_AUTOSKIP);
    field_opts_off(field[0], O_STATIC);
    set_max_field(field[0], 30);
	FORM *my_form = new_form(field);
	scale_form(my_form, &rows, &cols);
    WINDOW *my_form_win = newwin(rows + 4, cols + 4, termRow / 2 - (rows + 4) / 2, termCol / 2 - termCol / 8);
    PANEL *formPanel = new_panel(my_form_win);
    keypad(my_form_win, TRUE);
    set_form_win(my_form, my_form_win);
    set_form_sub(my_form, derwin(my_form_win, rows, cols, 2, 2));
    box(my_form_win, 0, 0);
    printCenter(my_form_win, message, 1, cols);
	post_form(my_form);
    show_panel(formPanel);
	updateScreen();
    wchar_t ch;
	/* Loop through to get user requests */
	while((ch = wgetch(my_form_win)) != 10)
	{
        switch(ch)
		{
            case KEY_LEFT:
			    form_driver(my_form, REQ_PREV_CHAR);
			    break;
			case KEY_RIGHT:
			    form_driver(my_form, REQ_NEXT_CHAR);
			    break;
            case KEY_BACKSPACE:
                form_driver(my_form, REQ_PREV_CHAR);
                form_driver(my_form, REQ_DEL_CHAR);
                break;
            case KEY_DC:
                form_driver(my_form, REQ_DEL_CHAR);
                break;
			default:
				form_driver(my_form, ch);
				break;
		}
	}
    form_driver(my_form, REQ_VALIDATION);
    std::string filename = field_buffer(field[0], 0);
    while(filename.back() == ' ')
		filename.pop_back();

    curs_set(FALSE);
    unpost_form(my_form);
	free_form(my_form);
	free_field(field[0]);
    hide_panel(formPanel);
    delwin(my_form_win);
    del_panel(formPanel);
    updateScreen();
    return filename;
}

void Controller::printBoard()
{
    WINDOW *win = panel_window(boardPanel);
    wmove(win, 1, 1);
    std::vector<std::vector<bool>> matrix = board->getMatrix();
    for (int r = 0; r < board->getHeight(); r++)
    {
        //std::string boardrow = "";
        for (int c=0; c < board->getWidth(); c++)
        {
            if(matrix[r][c])
            {
                waddch(win, 'X');
                //boardrow += "X";
            }
            else
            {
                waddch(win, ' ');
                //boardrow += " ";
            }

        }
        wmove(win, r+2, 1);
        //mvwprintw(win, r+1, (termCol - board->getWidth())/2, boardrow.c_str());
    }
    show_panel(boardPanel);
    updateScreen();
    wmove(win, 1, 1);
}

void Controller::runIteration()
{
    board->runIteration();
    updateStatusWin();
}

bool Controller::GetYesOrNo(std::string dialog)
{
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
	while((ch = wgetch(dialogWin)) != 10)
	{
        switch(ch)
	    {
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
    curs_set(FALSE);
    hide_panel(dialogPanel);
    delwin(dialogWin);
    del_panel(dialogPanel);
    updateScreen();
    return yesSelected;
}

void Controller::GetPatternDimensions(int &height, int &width)
{
    curs_set(TRUE);
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
    set_field_type(field[1], TYPE_INTEGER, 0, 1, 1000);
    field_opts_off(field[2], O_ACTIVE);
    set_field_buffer(field[2], 0, "Width: ");
    set_field_back(field[3], A_UNDERLINE);
	field_opts_off(field[3], O_AUTOSKIP);
    set_field_type(field[3], TYPE_INTEGER, 0, 1, 1000);
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
    post_form(form);
    show_panel(formPanel);
    updateScreen();
    int heightInput = 0, widthInput = 0;
    wchar_t ch;
    /* Loop through to get user requests */
    while((ch = wgetch(formWin)))
    {
        switch(ch)
        {
            case KEY_LEFT:
                form_driver(form, REQ_PREV_FIELD);
                break;
            case KEY_RIGHT:
                form_driver(form, REQ_NEXT_FIELD);
                break;
            case KEY_BACKSPACE:
                form_driver(form, REQ_PREV_CHAR);
                form_driver(form, REQ_DEL_CHAR);
                break;
            case KEY_DC:
                form_driver(form, REQ_DEL_CHAR);
                break;
            case 10:
                form_driver(form, REQ_VALIDATION);
                heightInput = atoi( field_buffer(field[1], 0) );
                widthInput = atoi( field_buffer(field[3], 0) );
                if(heightInput != 0 && widthInput != 0 )
                {
                    height = heightInput;
                    width = widthInput;
                    return;
                }
                break;
            default:
                form_driver(form, ch);
                break;
        }
    }
    curs_set(FALSE);
}

double Controller::getRatioInput()
{
    curs_set(TRUE);
    FIELD *field[2];
	int rows, cols;
	field[0] = new_field(1, termCol / 4, 1, 1, 0, 0);
	field[1] = NULL;
	set_field_back(field[0], A_UNDERLINE);
	field_opts_off(field[0], O_AUTOSKIP);
    field_opts_off(field[0], O_STATIC);
    set_field_type(field[0], TYPE_NUMERIC, 10, 0, 1);
    set_max_field(field[0], 10);
	FORM *my_form = new_form(field);
	scale_form(my_form, &rows, &cols);
    WINDOW *my_form_win = newwin(rows + 4, cols + 4, termRow / 2 - (rows + 4) / 2, termCol / 2 - termCol / 8);
    PANEL *formPanel = new_panel(my_form_win);
    keypad(my_form_win, TRUE);
    set_form_win(my_form, my_form_win);
    set_form_sub(my_form, derwin(my_form_win, rows, cols, 2, 2));
    box(my_form_win, 0, 0);
    printCenter(my_form_win, "Enter a ratio:", 1, cols);
	post_form(my_form);
    show_panel(formPanel);
	updateScreen();
    wchar_t ch;
	/* Loop through to get user requests */
    //bool isValid = false;
    double ratio = 0;
	while (ch != '\n')
	{
        ch = wgetch(my_form_win);
        switch(ch)
		{
            case KEY_LEFT:
			    form_driver(my_form, REQ_PREV_CHAR);
			    break;
			case KEY_RIGHT:
			    form_driver(my_form, REQ_NEXT_CHAR);
			    break;
            case KEY_BACKSPACE:
                form_driver(my_form, REQ_PREV_CHAR);
                form_driver(my_form, REQ_DEL_CHAR);
                break;
            case KEY_DC:
                form_driver(my_form, REQ_DEL_CHAR);
                break;
            case '\n':
                form_driver(my_form, REQ_VALIDATION);
                ratio = atof(field_buffer(field[0], 0));
			default:
				form_driver(my_form, ch);
				break;
		}
	}
    form_driver(my_form, REQ_VALIDATION);
    ratio = atof(field_buffer(field[0], 0));
    //while(filename.back() == ' ')
		//filename.pop_back();

    curs_set(FALSE);
    unpost_form(my_form);
	free_form(my_form);
	free_field(field[0]);
    hide_panel(formPanel);
    delwin(my_form_win);
    del_panel(formPanel);
    updateScreen();
    return ratio;
}

int Controller::getIntInput(std::string message)
{
    curs_set(TRUE);
    FIELD *field[2];
	int rows, cols;
	field[0] = new_field(1, termCol / 4, 1, 1, 0, 0);
	field[1] = NULL;
	set_field_back(field[0], A_UNDERLINE);
	field_opts_off(field[0], O_AUTOSKIP);
    field_opts_off(field[0], O_STATIC);
    set_field_type(field[0], TYPE_NUMERIC, 10, 0, 1);
    set_max_field(field[0], 10);
	FORM *my_form = new_form(field);
	scale_form(my_form, &rows, &cols);
    WINDOW *my_form_win = newwin(rows + 4, cols + 4, termRow / 2 - (rows + 4) / 2, termCol / 2 - termCol / 8);
    PANEL *formPanel = new_panel(my_form_win);
    keypad(my_form_win, TRUE);
    set_form_win(my_form, my_form_win);
    set_form_sub(my_form, derwin(my_form_win, rows, cols, 2, 2));
    box(my_form_win, 0, 0);
    printCenter(my_form_win, message.c_str(), 1, cols);
	post_form(my_form);
    show_panel(formPanel);
	updateScreen();
    wchar_t ch;
	/* Loop through to get user requests */
    //bool isValid = false;
    int num = 0;
	while (ch != '\n')
	{
        ch = wgetch(my_form_win);
        switch(ch)
		{
            case KEY_LEFT:
			    form_driver(my_form, REQ_PREV_CHAR);
			    break;
			case KEY_RIGHT:
			    form_driver(my_form, REQ_NEXT_CHAR);
			    break;
            case KEY_BACKSPACE:
                form_driver(my_form, REQ_PREV_CHAR);
                form_driver(my_form, REQ_DEL_CHAR);
                break;
            case KEY_DC:
                form_driver(my_form, REQ_DEL_CHAR);
                break;
            case '\n':
                form_driver(my_form, REQ_VALIDATION);
                num = atof(field_buffer(field[0], 0));
			default:
				form_driver(my_form, ch);
				break;
		}
	}
    form_driver(my_form, REQ_VALIDATION);
    num = atof(field_buffer(field[0], 0));
    //while(filename.back() == ' ')
		//filename.pop_back();

    curs_set(FALSE);
    unpost_form(my_form);
	free_form(my_form);
	free_field(field[0]);
    hide_panel(formPanel);
    delwin(my_form_win);
    del_panel(formPanel);
    updateScreen();
    return num;
}

void Controller::EditMode()
{
    WINDOW* boardWin = panel_window(boardPanel);
    keypad(boardWin, TRUE);
    int maxX = 0, maxY = 0;
    getmaxyx(boardWin, maxY, maxX);
    mvwaddch(boardWin, maxY / 2, maxX / 2, winch(boardWin)|A_STANDOUT);
    wmove(boardWin, maxY / 2, maxX / 2);
	wchar_t input = getch();
	int x = 0, y = 0;
	//while((input = wgetch(boardWin)) != 'p')
    while(getState() == paused)
    {
        updateScreen();
        input = getch();
        if( input == KEY_UP || input == KEY_DOWN || input == KEY_LEFT || input == KEY_RIGHT || input == ' ')
        {
            getyx(boardWin, y, x);
            waddch(boardWin, char( winch(boardWin) ));
            wmove(boardWin, y, x);
        }
        //wprintw(boardWin, "%d", y);
        //wprintw(boardWin, "%d", x);
        std::string filename = "";
        bool isFileValid = false;
        Pattern *pattern = nullptr;
        std::string layout = "";
        std::vector<std::vector<bool>> matrix;
        switch(input)
        {
            case KEY_UP:
                if(y == 1)
                {
                    wmove(boardWin, maxY - 2, x);
                }
                else
                {
                    wmove(boardWin, y-1, x);
                }
                getyx(boardWin, y, x);
                waddch(boardWin, winch(boardWin)|A_STANDOUT);
                wmove(boardWin, y, x);
                break;
            case KEY_DOWN:
                if(y == (maxY - 2))
                {
                    wmove(boardWin, 1, x);
                }
                else
                {
                    wmove(boardWin, y+1, x);
                }
                getyx(boardWin, y, x);
                waddch(boardWin, winch(boardWin)|A_STANDOUT);
                wmove(boardWin, y, x);
                break;
            case KEY_LEFT:
                if(x == 1)
                {
                    wmove(boardWin, y, maxX - 2);
                }
                else
                {
                    wmove(boardWin, y, x-1);
                }
                getyx(boardWin, y, x);
                waddch(boardWin, winch(boardWin)|A_STANDOUT);
                wmove(boardWin, y, x);
                break;
            case KEY_RIGHT:
                if(x == maxX - 2)
                {
                    wmove(boardWin, y, 1);
                }
                else
                {
                    wmove(boardWin, y, x+1);
                }
                getyx(boardWin, y, x);
                waddch(boardWin, winch(boardWin)|A_STANDOUT);
                wmove(boardWin, y, x);
                break;
            case ' ':
                board->toggle(y-1, x-1);
                printBoard();
                wmove(boardWin, y, x);
                waddch(boardWin, winch(boardWin)|A_STANDOUT);
                wmove(boardWin, y, x);
                break;
            case '[':
                setSpeed(-1);
                break;
            case ']':
                setSpeed(1);
                break;
            case 'a':
                getyx(boardWin, y, x);
                while(!isFileValid)
                {
                    filename = getStringInput("Enter a filename:");
                    try
                    {
                        pattern = new Pattern(filename);
                    }catch(char const*)
                    {
                        continue;
                    }
                    isFileValid = true;
                }
                matrix = pattern->getMatrix();
                RenderPattern(matrix);
                do
                {
                    input = wgetch(boardWin);
                    getyx(boardWin, y, x);
                    switch(input)
                    {
                        case KEY_UP:
                            if(y == 1)
                            {
                                wmove(boardWin, maxY - 2, x);
                            }
                            else
                            {
                                wmove(boardWin, y-1, x);
                            }
                            RenderPattern(matrix);
                            break;
                        case KEY_DOWN:
                            if(y == (maxY - 2))
                            {
                                wmove(boardWin, 1, x);
                            }
                            else
                            {
                                wmove(boardWin, y+1, x);
                            }
                            RenderPattern(matrix);
                            break;
                        case KEY_LEFT:
                            if(x == 1)
                            {
                                wmove(boardWin, y, maxX - 2);
                            }
                            else
                            {
                                wmove(boardWin, y, x-1);
                            }
                            RenderPattern(matrix);
                            break;
                        case KEY_RIGHT:
                            if(x == maxX - 2)
                            {
                                wmove(boardWin, y, 1);
                            }
                            else
                            {
                                wmove(boardWin, y, x+1);
                            }
                            RenderPattern(matrix);
                            break;
                        case '\'':
                            pattern->Rotate();
                            matrix = pattern->getMatrix();
                            RenderPattern(matrix);
                            break;
                        case ';':
                            pattern->Rotate();
                            pattern->Rotate();
                            pattern->Rotate();
                            matrix = pattern->getMatrix();
                            RenderPattern(matrix);
                            break;
                        case 10:
                            board->addPattern(pattern->getMatrix(), y, x);
                            break;
                    }
                } while(input != 10);
                werase(boardWin);
                box(boardWin, 0, 0);
                printBoard();
                wmove(boardWin, y, x);
                break;
            case 'p':
                setState(running);
                break;
            case 10:
                runIteration();
                printBoard();
                break;
            case 27:
                setState(menu);
                break;
        }
	}
}

void Controller::PatternEditor()
{
    WINDOW* boardWin = panel_window(boardPanel);
    keypad(boardWin, TRUE);
    int maxX = 0, maxY = 0;
    getmaxyx(boardWin, maxY, maxX);
    mvwaddch(boardWin, maxY / 2, maxX / 2, winch(boardWin)|A_STANDOUT);
    wmove(boardWin, maxY / 2, maxX / 2);
	wchar_t input;
    int x = 0, y = 0;
    while(state == editing)
    {
        updateScreen();
        input = getch();
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
            case KEY_UP:
                if(y == 1)
                {
                    wmove(boardWin, maxY - 2, x);
                }
                else
                {
                    wmove(boardWin, y-1, x);
                }
                getyx(boardWin, y, x);
                waddch(boardWin, winch(boardWin)|A_STANDOUT);
                wmove(boardWin, y, x);
                break;
            case KEY_DOWN:
                if(y == (maxY - 2))
                {
                    wmove(boardWin, 1, x);
                }
                else
                {
                    wmove(boardWin, y+1, x);
                }
                getyx(boardWin, y, x);
                waddch(boardWin, winch(boardWin)|A_STANDOUT);
                wmove(boardWin, y, x);
                break;
            case KEY_LEFT:
                if(x == 1)
                {
                    wmove(boardWin, y, maxX - 2);
                }
                else
                {
                    wmove(boardWin, y, x-1);
                }
                getyx(boardWin, y, x);
                waddch(boardWin, winch(boardWin)|A_STANDOUT);
                wmove(boardWin, y, x);
                break;
            case KEY_RIGHT:
                if(x == maxX - 2)
                {
                    wmove(boardWin, y, 1);
                }
                else
                {
                    wmove(boardWin, y, x+1);
                }
                getyx(boardWin, y, x);
                waddch(boardWin, winch(boardWin)|A_STANDOUT);
                wmove(boardWin, y, x);
                break;
            //consider making a function for adding the pattern
            case 'a':
                    getyx(boardWin, y, x);
                    while(!isFileValid)
                    {
                        filename = getStringInput("Enter a filename:");
                        try
                        {
                            pattern = new Pattern(filename);
                        }catch(char const*)
                        {
                            continue;
                        }
                        isFileValid = true;
                    }
                    matrix = pattern->getMatrix();
                    RenderPattern(matrix);
                    do
                    {
                        input = wgetch(boardWin);
                        getyx(boardWin, y, x);
                        switch(input)
                        {
                            case KEY_UP:
                                if(y == 1)
                                {
                                    wmove(boardWin, maxY - 2, x);
                                }
                                else
                                {
                                    wmove(boardWin, y-1, x);
                                }
                                RenderPattern(matrix);
                                break;
                            case KEY_DOWN:
                                if(y == (maxY - 2))
                                {
                                    wmove(boardWin, 1, x);
                                }
                                else
                                {
                                    wmove(boardWin, y+1, x);
                                }
                                RenderPattern(matrix);
                                break;
                            case KEY_LEFT:
                                if(x == 1)
                                {
                                    wmove(boardWin, y, maxX - 2);
                                }
                                else
                                {
                                    wmove(boardWin, y, x-1);
                                }
                                RenderPattern(matrix);
                                break;
                            case KEY_RIGHT:
                                if(x == maxX - 2)
                                {
                                    wmove(boardWin, y, 1);
                                }
                                else
                                {
                                    wmove(boardWin, y, x+1);
                                }
                                RenderPattern(matrix);
                                break;
                            case '\'':
                                pattern->Rotate();
                                matrix = pattern->getMatrix();
                                RenderPattern(matrix);
                                break;
                            case ';':
                                pattern->Rotate();
                                pattern->Rotate();
                                pattern->Rotate();
                                matrix = pattern->getMatrix();
                                RenderPattern(matrix);
                                break;
                            case 10:
                                board->addPattern(pattern->getMatrix(), y, x);
                                break;
                        }
                    } while(input != 10);
                    werase(boardWin);
                    box(boardWin, 0, 0);
                    printBoard();
                    wmove(boardWin, y, x);
                    break;
            case ' ':
                board->toggle(y-1, x-1);
                printBoard();
                wmove(boardWin, y, x);
                waddch(boardWin, winch(boardWin)|A_STANDOUT);
                wmove(boardWin, y, x);
                break;
            case 27:
                setState(menu);
                break;
        }
    }
}

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

void Controller::SaveCurrent(bool isPattern)
{
    bool shouldSave = GetYesOrNo("Would you like to save?");
    if(shouldSave)
    {
        std::string filename = getStringInput("Enter a filename:");
        if(isPattern)
        {
            filename = "patterns/" + filename;
        }
        else
        {
            filename = "boards/" + filename;
        }
        board->saveState(filename);
    }
}

bool Controller::isSaved()
{
    return board->getIsSaved();
}

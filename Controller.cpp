#include "Controller.h"

#define MAIN_MENU_HEIGHT 7
#define MAIN_MENU_WIDTH 28
#define BOARD_HEIGHT termRow * .9
#define BOARD_WIDTH termCol

Controller::Controller()
{
    int maxY = 0, maxX = 0;
    getmaxyx(stdscr, maxY, maxX);
    termRow = maxY;
    termCol = maxX;
    speed = 1000;
    state = "Loading";
    board = new Board(true, BOARD_HEIGHT - 2, BOARD_WIDTH - 2);
    //Create window for the board
    WINDOW *boardWin = newwin(BOARD_HEIGHT, BOARD_WIDTH, 0, 0);
    boardPanel = new_panel(boardWin);
    box(boardWin, 0, 0);
    mvwprintw(boardWin, 1, 1, "+");
    //Create window for the bottom status area
    WINDOW *statusWin = newwin(termRow - BOARD_HEIGHT + 1, BOARD_WIDTH, BOARD_HEIGHT, 0);
    statusPanel = new_panel(statusWin);
    box(statusWin, 0, 0);
    updateStatusWin();
    updateScreen();
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
    ITEM **choices = new ITEM*[5];
    choices[0] = new_item("Load a new board", "Load a new board");
    choices[1] = new_item("Load a saved board", "Load a saved board");
    choices[2] = new_item("Load a random board", "Load a random board");
    choices[3] = new_item("Load the pattern editor", "Load the pattern editor");
    choices[4] = NULL;
	/* Crate menu */
	MENU *my_menu = new_menu(choices);
    menu_opts_off(my_menu, O_NONCYCLIC);
    menu_opts_off(my_menu, O_SHOWDESC);

	/* Create the window to be associated with the menu */
    WINDOW *my_menu_win = newwin(MAIN_MENU_HEIGHT, MAIN_MENU_WIDTH,
                                    termRow / 2 - MAIN_MENU_HEIGHT / 2, termCol / 2 - MAIN_MENU_WIDTH / 2);
    keypad(my_menu_win, TRUE);
    PANEL *menuPanel = new_panel(my_menu_win);
     
	/* Set main window and sub window */
    set_menu_win(my_menu, my_menu_win);
    set_menu_sub(my_menu, derwin(my_menu_win, 4, 25, 2, 1));

	/* Set menu mark to the string " * " */
    set_menu_mark(my_menu, ">");
    printCenter(my_menu_win, "Main Menu", 1, 28);
	/* Print a border around the main window and print a title */
    box(my_menu_win, 0, 0);
	//print_in_middle(my_menu_win, 1, 0, 40, "My Menu", COLOR_PAIR(1));
	refresh();
        
	/* Post the menu */
	post_menu(my_menu);
    show_panel(menuPanel);
	wrefresh(my_menu_win);
    wchar_t ch;
	while((ch = wgetch(my_menu_win)) != 10)
	{       switch(ch)
	        {	case KEY_DOWN:
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
		}
                wrefresh(my_menu_win);
	}	

	/* Unpost and free all the memory taken up */
    int choice = item_index(current_item(my_menu));
    unpost_menu(my_menu);
    free_menu(my_menu);
    for(int i = 0; i < 5; ++i)
            free_item(choices[i]);
    hide_panel(menuPanel);
    updateScreen();
    return choice;
}

void Controller::createNewBoard(bool wrapAround)
{
    delete board;
    board = new Board(wrapAround, BOARD_HEIGHT - 2, BOARD_WIDTH - 2);
}

void Controller::createNewBoard(std::string filename)
{
    delete board;
    board = new Board(filename);
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

std::string Controller::getState()
{
    return state;
}

void Controller::setState(std::string newState)
{
    state = newState;
    updateStatusWin();
}

void Controller::setSpeed(int newSpeed)
{
    speed = newSpeed;
    updateStatusWin();
}

void Controller::updateStatusWin()
{
    WINDOW* statusWin = panel_window(statusPanel);
    mvwprintw(statusWin, 1, 1, "Board Size:\tStatus:\tSpeed:");
    mvwprintw(statusWin, 3, 1, "%d", board->getHeight());
    wprintw(statusWin, " x ");
    wprintw(statusWin, "%d", board->getWidth());
    wprintw(statusWin, "\t");
    wprintw(statusWin, state.c_str());
    wprintw(statusWin, "\t");
    wprintw(statusWin, "%.2f", speed/1000);
    updateScreen();
}

std::string Controller::getStringInput()
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
    printCenter(my_form_win, "Enter a filename:", 1, cols);
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
    curs_set(FALSE);
    unpost_form(my_form);
	free_form(my_form);
	free_field(field[0]);
    hide_panel(formPanel);
    delwin(my_form_win);
    del_panel(formPanel);
    updateScreen();
    return field_buffer(field[0], 0);
}
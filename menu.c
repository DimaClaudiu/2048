#include "menu.h"
#include "game.h"

#define WIDTH 30
#define HEIGHT 10

void initializeWindow()
{
	srand(time(NULL)); //random seed

	initscr();
	noecho(); //so getch won't echo the characters
	cbreak();	//disables line buffering
	curs_set(0); //hideing the annoying cursor

	start_color();						  //to be called when I want to use colors
	init_pair(1, COLOR_RED, COLOR_BLACK); //text configs
	attron(A_BOLD);

	clear();
	color_set(1, NULL); //initializeing colors
}

void drawMenu(WINDOW *win_menu, int s, char *menu_strings[], int n_strings)
{
	int x, y;

	x = 2; //menu strings origin
	y = 2;

	wclear(win_menu);

	box(win_menu, 0, 0); //outlineing the window
	init_pair(6, COLOR_BLUE, COLOR_BLACK);
	wbkgd(win_menu, COLOR_PAIR(6)); //initializeing colors

	for (int i = 0; i < n_strings; i++)
	{
		if (s == i)
		{
			mvwprintw(win_menu, y, x, ">%s", menu_strings[i]); // adding ">" to the selected string
		}
		else
			mvwprintw(win_menu, y, x, "%s", menu_strings[i]); //drawing the strings
		y++;
	}
	
	refresh();
}

void menu(int resumeable)
{
	WINDOW *win_menu;
	int selected = 0; //curently selected string index
	int key = -1; //last key pressed

	char *menu_strings[] = {
		"New Game",
		"Resume",
		"Quit",
	};
	int n_strings = sizeof(menu_strings) / sizeof(char *); //basically 3, but in case I'll add more strings

	win_menu = newwin(HEIGHT, WIDTH, (24 - HEIGHT) / 2, (80 - WIDTH) / 2); //creating the menu window
	keypad(win_menu, TRUE); //enable the keypad inside the window
	mvprintw(0, 80 / 2 - 8, "Welcome to 2048!"); //some message
	refresh();
	drawMenu(win_menu, selected, menu_strings, n_strings); //initially drawing the menu

	while (1) //updateting the menu
	{
		key = wgetch(win_menu); //awating input
		if (key == KEY_UP) //moving up
		{
			if (selected != 0)
			{
				selected--;
			}
			else
			{
				selected = n_strings - 1; //going around
			}
		}
		if (key == KEY_DOWN) //moving down
		{
			if (selected != n_strings - 1)
			{
				selected++;
			}
			else
			{
				selected = 0; //going around
			}
		}
		if (key == 10) //enter was pressed
		{
			if (selected == 0) //new game
			{
				initGame();
			}
			if (selected == 1 && resumeable) //if we have what, we'll resume
			{
				clear();
				refresh();
				return;
			}
			else if(selected == n_strings - 1) //quit will always be last
			{
				endwin();
				exit(1); //exiting
			}
		}

		drawMenu(win_menu, selected, menu_strings, n_strings); //redrawing
	}
}
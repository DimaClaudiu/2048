#ifndef MENU_H_
#define MENU_H_

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

void initializeWindow();

void drawMenu(WINDOW *win_menu, int s, char *menu_strings[], int n_strings);

void menu(int resumeable);

#endif //MENU_H_
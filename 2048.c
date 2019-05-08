#include "game.h"
#include "menu.h"

int main(void)
{
	initializeWindow(); //doing all the initialization at the start
	menu(0); //staring with the menu and a non resumeable game, the rest goes from there

	return 0; //won't ever come to this but oh well
}

#ifndef GAME_H_
#define GAME_H_

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

typedef struct gameboard
{
	unsigned int board[4][4];
	unsigned int score;
	int lost;
} GameBoard;

void initGame();

void game(WINDOW *win_game, WINDOW *win_panel, GameBoard gameBoard);

int updateGame(int dir, GameBoard *gameBoard);

void addPiece(unsigned int a[4][4]);

void clearCk(unsigned int ck[][4]);

void swap(int *a, int *b);

int moveUp(unsigned int a[][4], unsigned int *score);

int moveDown(unsigned int a[][4], unsigned int *score);

int moveRight(unsigned int a[][4], unsigned int *score);

int moveLeft(unsigned int a[][4], unsigned int *score);

void changeColor(int r, int g, int b, int color);

void drawPiece(WINDOW *win_game, int n, int x, int y);

void drawGame(WINDOW *win_game, GameBoard gameBoard);

int drawPanel(WINDOW *win_panel, unsigned int score, char moves[], int won);

void copyMatrix(unsigned int a[4][4], unsigned int b[4][4]);

int checkWon(unsigned int a[4][4]);

char *checkLost(unsigned int a[4][4]);

short countBlanks(unsigned int a[4][4]);

void executeBestMove(GameBoard *gameBoard);

#endif //GAME_H_
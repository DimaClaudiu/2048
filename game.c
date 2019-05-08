#include "game.h"
#include "menu.h"

void initGame()
{
    GameBoard gameBoard;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            gameBoard.board[i][j] = 0; //emptying the board

    addPiece(gameBoard.board); //adding 2 random pieces at the start
    addPiece(gameBoard.board);

    gameBoard.score = 0; //the score stars at 0
    gameBoard.lost = 0;  //I guess we didn't lose right from the start

    WINDOW *win_game; //creating the main game window
    win_game = newwin(14, 35, 1, 1);

    keypad(win_game, TRUE);
    nodelay(win_game, TRUE); //for getch inside while as we count passing time

    clear();
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    wbkgd(win_game, COLOR_PAIR(2)); //all the coloring

    WINDOW *win_panel; //same but for the panel window
    win_panel = newwin(10, 30, 1, 45);
    wbkgd(win_panel, COLOR_PAIR(2));

    drawGame(win_game, gameBoard);                                     //drawing the board
    drawPanel(win_panel, gameBoard.score, checkLost(gameBoard.board), 0); //drawing the panel
    game(win_game, win_panel, gameBoard);                              //starting the game!
}

void game(WINDOW *win_game, WINDOW *win_panel, GameBoard gameBoard)
{
    int key;           //last pressed key
    float seconds = 0; //passed sconds
    int won = 0; //you won't win right at the start^^

    while (1) //main loop
    {
        key = wgetch(win_game);                                                      //awating input
        if (key == KEY_UP || key == KEY_DOWN || key == KEY_RIGHT || key == KEY_LEFT) //valid move key
        {
            seconds = 0;
            won = updateGame(key, &gameBoard);
            drawGame(win_game, gameBoard);
            if (drawPanel(win_panel, gameBoard.score, checkLost(gameBoard.board), won))
            {
                //losing sequence
                while (1)
                {
                    key = wgetch(win_game);
                    if (key == 'q' || key == 'Q')
                    {
                        exit(0);
                    }
                }
            }
        }
        else if (key == 'q' || key == 'Q') //we want to return
        {
            clear();
            refresh();
            menu(1);                       //going back inside menu's main loop
            drawGame(win_game, gameBoard); //we returned from menu();, now we're back here
            drawPanel(win_panel, gameBoard.score, checkLost(gameBoard.board), won);
        }

        usleep(100 * 1000); //good enough time counting
        seconds += 0.1;

        if (seconds >= 10)
        {
            executeBestMove(&gameBoard);
            drawGame(win_game, gameBoard);
            if (drawPanel(win_panel, gameBoard.score, checkLost(gameBoard.board), won))
            {
                //losing sequence
            }
            seconds = 0;
        }
    }
}

int updateGame(int dir, GameBoard *gameBoard)
{
    if (dir == KEY_UP)
    {
        if (moveUp(gameBoard->board, &(gameBoard->score))) //if the move is valid
            addPiece(gameBoard->board);                    //we add a piece
    }
    else if (dir == KEY_DOWN) //same for the rest
    {
        if (moveDown(gameBoard->board, &(gameBoard->score)))
            addPiece(gameBoard->board);
    }
    else if (dir == KEY_RIGHT)
    {
        if (moveRight(gameBoard->board, &(gameBoard->score)))
            addPiece(gameBoard->board);
    }
    else if (dir == KEY_LEFT)
    {
        if (moveLeft(gameBoard->board, &(gameBoard->score)))
            addPiece(gameBoard->board);
    }

    return checkWon(gameBoard->board); //perhaps we added up to 2048 or greater
}

void addPiece(unsigned int a[4][4])
{
    //There will always be atleast 1 free spot to place a piece since we checkLost after each add
    int rX = rand() % 4;
    int rY = rand() % 4;
    while (a[rX][rY] != 0) //we try until we succeed
    {
        rX = rand() % 4;
        rY = rand() % 4;
    }

    if (rand() % 10 < 9) //90% for a 2
    {
        a[rX][rY] = 2;
    }
    else //10% for a 4
    {
        a[rX][rY] = 4;
    }
}

void clearCk(unsigned int ck[][4])
{
    //tidier way for clearing the checked matrix
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ck[i][j] = 0;
}

void swap(int *a, int *b)
{
    //simple swaping of 2 integers
    int aux = *a;
    *a = *b;
    *b = aux;
}

int moveUp(unsigned int a[][4], unsigned int *score)
{
    unsigned int ck[4][4];                     //to be used for checking if the move is valid
    int moved = 0;                             //so we know if we moved anything
    clearCk(ck);                               //nothing check at the start
    for (int collum = 0; collum < 4; collum++) //going collum by collum
    {
        for (int i = 1; i < 4; i++) //starting with the second top-most piece, then taking the rest
        {

            for (int j = i; j > 0; j--) //going up
            {
                if (a[j][collum] == 0) //no reason to move an empty place
                {
                    break;
                }
                if (a[j - 1][collum] == 0) //if it's clear above, we  move up
                {
                    a[j - 1][collum] = a[j][collum];
                    a[j][collum] = 0;
                    ck[j - 1][collum] = ck[j][collum];
                    ck[j][collum] = 0;
                    moved = 1;
                }
                else if (a[j - 1][collum] == a[j][collum] && ck[j][collum] == 0 && ck[j - 1][collum] == 0) //if it's the same piece, and we didn't already combine, we combine and move up
                {
                    a[j - 1][collum] *= 2;
                    *score += a[j - 1][collum];
                    a[j][collum] = 0;
                    ck[j - 1][collum] = 1;
                    a[j][collum] = 0;
                    moved = 1;
                }
                else //then it means it's either a different piece, or we already combined, so we stay put
                {
                    break;
                }
            }
        }
    }
    return moved; //returning wheter we made a valid move or not
}

int moveDown(unsigned int a[][4], unsigned int *score)
{
    //same as moving up but doing all the checks starting from the second bottom-most piece and moving up
    unsigned int ck[4][4];
    int moved = 0;
    clearCk(ck);
    for (int collum = 0; collum < 4; collum++)
    {
        for (int i = 3; i >= 0; i--)
        {

            for (int j = i; j < 3; j++)
            {
                if (a[j][collum] == 0)
                {
                    break;
                }
                if (a[j + 1][collum] == 0)
                {
                    a[j + 1][collum] = a[j][collum];
                    a[j][collum] = 0;
                    ck[j + 1][collum] = ck[j][collum];
                    ck[j][collum] = 0;
                    moved = 1;
                }
                else if (a[j + 1][collum] == a[j][collum] && ck[j][collum] == 0 && ck[j + 1][collum] == 0)
                {
                    a[j + 1][collum] *= 2;
                    *score += a[j + 1][collum];
                    a[j][collum] = 0;
                    ck[j + 1][collum] = 1;
                    a[j][collum] = 0;
                    moved = 1;
                }
                else
                {
                    break;
                }
            }
        }
    }
    return moved;
}

int moveRight(unsigned int a[][4], unsigned int *score)
{
    //same as moving up execpt we go line by line now starting from the second right-most piece and moving left
    unsigned int ck[4][4];
    int moved = 0;
    clearCk(ck);
    for (int line = 0; line < 4; line++)
    {
        for (int i = 3; i >= 0; i--)
        {

            for (int j = i; j < 3; j++)
            {
                if (a[line][j] == 0)
                {
                    break;
                }
                if (a[line][j + 1] == 0)
                {
                    a[line][j + 1] = a[line][j];
                    a[line][j] = 0;
                    ck[line][j + 1] = ck[line][j + 1];
                    ck[line][j] = 0;
                    moved = 1;
                }
                else if (a[line][j + 1] == a[line][j] && ck[line][j] == 0 && ck[line][j + 1] == 0)
                {
                    a[line][j + 1] *= 2;
                    *score += a[line][j + 1];
                    a[line][j] = 0;
                    ck[line][j + 1] = 1;
                    a[line][j] = 0;
                    moved = 1;
                }
                else
                {
                    break;
                }
            }
        }
    }
    return moved;
}

int moveLeft(unsigned int a[][4], unsigned int *score)
{
    //same as moving up execpt we go line by line now starting from the second left-most piece and moving right
    unsigned int ck[4][4];
    int moved = 0;
    clearCk(ck);
    for (int line = 0; line < 4; line++)
    {
        for (int i = 1; i < 4; i++)
        {

            for (int j = i; j > 0; j--)
            {
                if (a[line][j] == 0)
                {
                    break;
                }
                if (a[line][j - 1] == 0)
                {
                    a[line][j - 1] = a[line][j];
                    a[line][j] = 0;
                    ck[line][j - 1] = ck[line][j - 1];
                    ck[line][j] = 0;
                    moved = 1;
                }
                else if (a[line][j - 1] == a[line][j] && ck[line][j] == 0 && ck[line][j - 1] == 0)
                {
                    a[line][j - 1] *= 2;
                    *score += a[line][j - 1];
                    a[line][j] = 0;
                    ck[line][j - 1] = 1;
                    a[line][j] = 0;
                    moved = 1;
                }
                else
                {
                    break;
                }
            }
        }
    }
    return moved;
}

void drawPiece(WINDOW *win_game, int n, int x, int y)
{
    //initalizeing different pairs of colors for each piece
    init_color(9, 203 * 200 / 51, 192 * 200 / 51, 177 * 200 / 51);
    init_pair(3, COLOR_BLACK, 9); //0
    init_color(10, 238 * 200 / 51, 228 * 200 / 51, 177 * 217 / 51);
    init_pair(4, COLOR_BLACK, 10); //2
    init_color(11, 234 * 200 / 51, 206 * 200 / 51, 174 * 200 / 51);
    init_pair(5, COLOR_BLACK, 11); //4
    init_color(12, 239 * 200 / 51, 178 * 200 / 51, 112 * 200 / 51);
    init_pair(6, COLOR_BLACK, 12); //8
    init_color(13, 252 * 200 / 51, 146 * 200 / 51, 22 * 200 / 51);
    init_pair(7, COLOR_BLACK, 13); //16
    init_color(14, 242 * 200 / 51, 126 * 200 / 51, 88 * 200 / 51);
    init_pair(8, COLOR_BLACK, 14); //32
    init_color(15, 241 * 200 / 51, 47 * 200 / 51, 47 * 200 / 51);
    init_pair(9, COLOR_BLACK, 15); //64
    init_color(16, 253 * 200 / 51, 221 * 200 / 51, 80 * 200 / 51);
    init_pair(10, COLOR_BLACK, 16); //128
    init_color(21, 236 * 200 / 51, 162 * 200 / 51, 45 * 200 / 51);
    init_pair(15, COLOR_BLACK, 21); //256
    init_color(17, 246 * 200 / 51, 201 * 200 / 51, 0 * 200 / 51);
    init_pair(11, COLOR_BLACK, 17); //512
    init_color(18, 245 * 200 / 51, 81 * 200 / 51, 197 * 200 / 51);
    init_pair(12, COLOR_BLACK, 18); //1024
    init_color(19, 68 * 200 / 51, 126 * 200 / 51, 242 * 200 / 51);
    init_pair(13, COLOR_BLACK, 19); //2048
    init_color(20, 60 * 200 / 51, 60 * 200 / 51, 60 * 200 / 51);
    init_pair(14, COLOR_WHITE, 20); //larger

    //and attributing then for each specific piece, then drawing
    if (n == 0)
    {
        wattron(win_game, COLOR_PAIR(3));
        mvwprintw(win_game, y, x, "      ", n);
    }
    else if (n == 2)
    {
        wattron(win_game, COLOR_PAIR(4));
        mvwprintw(win_game, y, x, "  %d   ", n);
    }
    else if (n == 4)
    {
        wattron(win_game, COLOR_PAIR(5));
        mvwprintw(win_game, y, x, "  %d   ", n);
    }
    else if (n == 8)
    {
        wattron(win_game, COLOR_PAIR(6));
        mvwprintw(win_game, y, x, "  %d   ", n);
    }
    else if (n == 16)
    {
        wattron(win_game, COLOR_PAIR(7));
        mvwprintw(win_game, y, x, "  %d  ", n);
    }
    else if (n == 32)
    {
        wattron(win_game, COLOR_PAIR(8));
        mvwprintw(win_game, y, x, "  %d  ", n);
    }
    else if (n == 64)
    {
        wattron(win_game, COLOR_PAIR(9));
        mvwprintw(win_game, y, x, "  %d  ", n);
    }
    else if (n == 128)
    {
        wattron(win_game, COLOR_PAIR(10));
        mvwprintw(win_game, y, x, " %d  ", n);
    }
    else if (n == 256)
    {
        wattron(win_game, COLOR_PAIR(15));
        mvwprintw(win_game, y, x, " %d  ", n);
    }
    else if (n == 512)
    {
        wattron(win_game, COLOR_PAIR(11));
        mvwprintw(win_game, y, x, " %d  ", n);
    }
    else if (n == 1024)
    {
        wattron(win_game, COLOR_PAIR(12));
        mvwprintw(win_game, y, x, " %d ", n);
    }
    else if (n == 2048)
    {
        wattron(win_game, COLOR_PAIR(13));
        mvwprintw(win_game, y, x, " %d ", n);
    }
    else
    {
        wattron(win_game, COLOR_PAIR(14));
        mvwprintw(win_game, y, x, "%d", n);
    }
}

void drawGame(WINDOW *win_game, GameBoard gameBoard)
{
    int x = 2; //origins
    int y = 2;

    wclear(win_game);
    init_pair(5, COLOR_RED, COLOR_BLACK);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            //basically drawing a matrix with a bit spaced out characters
            drawPiece(win_game, gameBoard.board[i][j], x + 8 * j, y + 3 * i);
        }
        wattron(win_game, COLOR_PAIR(5));
    }
    box(win_game, 0, 0); //drawing the box after to avoid newlines overlapping
    refresh();
    wrefresh(win_game); //all the refreshing needed
}

int drawPanel(WINDOW *win_panel, unsigned int score, char moves[], int won)
{
    time_t t = time(NULL);
    struct tm time = *localtime(&t); //in order to get the current time and date

    wclear(win_panel);

    //more pais to make it colorful
    init_pair(16, COLOR_RED, COLOR_WHITE);   //losing text
    init_pair(17, COLOR_GREEN, COLOR_WHITE); //avalible moves text
    init_pair(18, COLOR_BLACK, COLOR_WHITE); //score, date and time

    wattron(win_panel, COLOR_PAIR(18));
    box(win_panel, 0, 0);

    mvwprintw(win_panel, 1, 2, "Time: %d:%d", time.tm_hour, time.tm_min);
    mvwprintw(win_panel, 2, 2, "Date: %d.%d.%d", time.tm_mday, time.tm_mon + 1, time.tm_year + 1900);

    mvwprintw(win_panel, 4, 2, "Score: %d", score);


    int lost = 0;          //returnig wheter we lost when calculating avalible moves
    if (strlen(moves) > 2) //if there are any moves
    {
        mvwprintw(win_panel, 6, 2, "Moves:");
        wattron(win_panel, COLOR_PAIR(17));
        mvwprintw(win_panel, 6, 8, " %s", moves + 1); //we draw them
    }
    else //no moves left, means we lost :(
    {

        wattron(win_panel, COLOR_PAIR(16));

        mvwprintw(win_panel, 7, 2, "YOU LOST! :(");
        mvwprintw(win_panel, 8, 2, "q - return to main menu");
        lost = 1;
    }

    if(won && !lost) //we can win and still keep going until we lose
    {
        mvwprintw(win_panel, 7, 2, "YOU WON! :)");
        mvwprintw(win_panel, 8, 2, "You can keep playing");
    }

    refresh();
    wrefresh(win_panel);
    return lost; //returning wether we lost or not
}

void copyMatrix(unsigned int a[4][4], unsigned int b[4][4])
{
    //for checking the validity of a move without modifying the main board
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            b[i][j] = a[i][j];
    }
}

char *checkLost(unsigned int a[4][4])
{
    //checking the avalible moves and returning them
    unsigned int aux[4][4]; //so we don't modify the main board
    static char moves[20];  //here we keep the moves as strings
    strcpy(moves, "");
    int s = 2; //paper weight

    copyMatrix(a, aux);
    if (moveUp(aux, &s)) //check each move
    {
        strcat(moves, ",Up"); //adding it in case it's valid
    }
    copyMatrix(a, aux);
    if (moveDown(aux, &s))
    {
        strcat(moves, ",Down");
    }
    copyMatrix(a, aux);
    if (moveRight(aux, &s))
    {
        strcat(moves, ",Right");
    }
    copyMatrix(a, aux);
    if (moveLeft(aux, &s))
    {
        strcat(moves, ",Left");
    }

    return moves; //returning avalible moves, it there are none, it means we lost
}

int checkWon(unsigned int a[4][4])
{
    //checking for a 2048 tile
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (a[i][j] >= 2048) 
                return 1;
    return 0;
}

short countBlanks(unsigned int a[4][4])
{
    //couting the free spaces inside our board, to be used with the automatic moves
    short blanks = 0;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (a[i][j] == 0)
                blanks++;

    return blanks;
}

void executeBestMove(GameBoard *gameBoard)
{
    unsigned int aux[4][4]; //so we don't modify the main board
    short blanks[4];        //the number of blanck for each move
    int s = 0;              //paper weight

    for (int i = 0; i < 4; i++)
        blanks[i] = -1; //just in case

    copyMatrix(gameBoard->board, aux);
    if (moveUp(aux, &s))
        blanks[0] = countBlanks(aux); //couting the blanks for each move

    copyMatrix(gameBoard->board, aux);
    if (moveDown(aux, &s))
        blanks[1] = countBlanks(aux);

    copyMatrix(gameBoard->board, aux);
    if (moveRight(aux, &s))
        blanks[2] = countBlanks(aux);

    copyMatrix(gameBoard->board, aux);
    if (moveLeft(aux, &s))
        blanks[3] = countBlanks(aux);

    short max = -1;
    int index;
    for (int i = 0; i < 4; i++) //checking wich move leads to the most blancks
    {
        if (blanks[i] > max)
        {
            max = blanks[i];
            index = i;
        }
    }

    if (index == 0) //executing that move accordingly
    {
        updateGame(KEY_UP, gameBoard);
    }
    else if (index == 1)
    {
        updateGame(KEY_DOWN, gameBoard);
    }
    else if (index == 2)
    {
        updateGame(KEY_RIGHT, gameBoard);
    }
    else if (index == 3)
    {
        updateGame(KEY_LEFT, gameBoard);
    }
}
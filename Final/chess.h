#ifndef __HEADER_CHESS_
#define __HEADER_CHESS_

// Board & Chat print function.
void print_board();
void clear_board();

// Movement Function. if impossible, return -1. else, move piece and return 1
// IF KING CAPTURED, RETURN 100*PLAYER AND GAME OVER
int input(char move[], int player);
int king(int sx, int sy, int ex, int ey);
int queen(int sx, int sy, int ex, int ey);
int bishop(int sx, int sy, int ex, int ey);
int knight(int sx, int sy, int ex, int ey);
int rook(int sx, int sy, int ex, int ey);
int pawn(int sx, int sy, int ex, int ey);
#endif
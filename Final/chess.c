#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KING 12
#define QUEEN 10
#define BISHOP 8
#define KNIGHT 6
#define ROOK 4
#define PAWN 2

int board[8][8] = {
    {-ROOK, -KNIGHT,-BISHOP,-QUEEN, -KING,  -BISHOP,-KNIGHT,-ROOK},
    {-PAWN, -PAWN,  -PAWN,  -PAWN,  -PAWN,  -PAWN,  -PAWN,  -PAWN},
    {0,     0,      0,      0,      0,      0,      0,      0   },
    {0,     0,      0,      0,      0,      0,      0,      0   },
    {0,     0,      0,      0,      0,      0,      0,      0   },
    {0,     0,      0,      0,      0,      0,      0,      0   },
    {PAWN,  PAWN,   PAWN,   PAWN,   PAWN,   PAWN,   PAWN,   PAWN},
    {ROOK,  KNIGHT, BISHOP, QUEEN,  KING,   BISHOP, KNIGHT, ROOK}
};

void print_board();

// if impossible, return -1. else, move piece and return 1
int input(char move[], int player);
int king(int sx, int sy, int ex, int ey);
int queen(int sx, int sy, int ex, int ey);
int bishop(int sx, int sy, int ex, int ey);
int knight(int sx, int sy, int ex, int ey);
int rook(int sx, int sy, int ex, int ey);
int pawn(int sx, int sy, int ex, int ey);

int main(int argc, char *argv[]){
    char str[1024];
    int player = 1, state = 1;
    print_board();

    while(state){
        scanf("%s", str);
        if(input(str, player) < 0){
            printf("invalid move\n");
        }
        print_board();
        player = player * (-1);
    }

    return 0;
}

void print_board(){
    int row, col;

    fprintf(stdout, " ");
    for(col = 0 ; col < 8; ++col){
        fprintf(stdout, "   %c", 'a'+col);
    }
    fprintf(stdout, "\n");
    for(row = 0; row < 8; ++row){
        fprintf(stdout, "\n%d", row+1);
        for(col = 0; col < 8; ++col){
            fprintf(stdout, "   ");
            switch(board[row][col]){
                case KING :
                    fprintf(stdout, "K");
                    break;
                case -KING:
                    fprintf(stdout, "k");
                    break;
                case QUEEN :
                    fprintf(stdout, "Q");
                    break;
                case -QUEEN:
                    fprintf(stdout, "q");
                    break;
                case BISHOP:
                    fprintf(stdout, "B");
                    break;
                case -BISHOP:
                    fprintf(stdout, "b");
                    break;
                case KNIGHT:
                    fprintf(stdout, "N");
                    break;
                case -KNIGHT:
                    fprintf(stdout, "n");
                    break;
                case ROOK : 
                    fprintf(stdout, "R");
                    break;
                case -ROOK :
                    fprintf(stdout, "r");
                    break;
                case PAWN:
                    fprintf(stdout, "P");
                    break;
                case -PAWN:
                    fprintf(stdout, "p");
                    break;
            }
        }
        fprintf(stdout, "\n");
    }
}

int input(char move[], int player){
    // move example e7e5 -> move piece from e7 to e5
    int sx, sy, ex, ey;

    if(strlen(move) != 4) return -1;

    sx = move[0] - 'a';
    sy = move[1] - '1';

    ex = move[2] - 'a';
    ey = move[3] - '1';

    if((sx > 7) || (sx < 0)) return -1;
    if((sy > 7) || (sy < 0)) return -1;

    if((ex > 7) || (ex < 0)) return -1;
    if((ey > 7) || (ey < 0)) return -1;

    if(player * board[sy][sx] <= 0) return -1;
    if(player * board[ey][ex] > 0) return -1;

    switch(abs(board[sy][sx])){
        case KING :
            if(king(sx, sy, ex, ey) > 0) return 1;
            else return -1;
        case QUEEN :
            if(queen(sx, sy, ex, sy) > 0) return 1;
            else return -1;
        case BISHOP:
            if(bishop(sx, sy, ex, ey) > 0) return 1;
            else return -1;
        case KNIGHT:
            if(knight(sx, sy, ex, ey) > 0) return 1;
            else return -1;
        case ROOK : 
            if(rook(sx, sy, ex, ey) > 0) return 1;
            else return -1;
        case PAWN:
            if(pawn(sx, sy, ex, ey) > 0) return 1;
            else return -1;
    }
    return -1;
}

int king(int sx, int sy, int ex, int ey){
    int king_vec[8][2] = {
        {-1, -1}, { 0, -1}, { 1, -1},
        {-1,  0},           { 1,  0},
        {-1,  1}, { 0,  1}, { 1,  1}
    };
    int i, j, dx, dy;
    int player = board[sy][sx] / KING;

    dx = ex - sx;
    dy = ey - ex;

    for(i = 0; i < 8; ++i){
        if((dx == king_vec[i][0]) && (dy == king_vec[i][1])){
            board[sy][sx] = 0;
            board[ey][ex] = KING * player;
            return 1;
        }
    }
    return -1;
}
int queen(int sx, int sy, int ex, int ey){
    int queen_vec[8][2] = {
        {-1, -1}, { 0, -1}, { 1, -1},
        {-1,  0},           { 1,  0},
        {-1,  1}, { 0,  1}, { 1,  1}
    };
    int i, j, dx, dy;
    int player = board[sy][sx] / QUEEN;

    dx = ex - sx;
    dy = ey - ex;

    for(i = 0; i < 8; ++i){
        if((dx%queen_vec[i][0] == 0) && (dy%queen_vec[i][1] == 0)){
            j = 1;
            while((sx+queen_vec[i][0]*j != ex) && (sy+queen_vec[i][1]*j != ey)){
                if(board[sy+queen_vec[i][1]*j][sx+queen_vec[i][0]*j] != 0) return -1;
                j++;
            }
            board[sy][sx] = 0;
            board[ey][ex] = QUEEN * player;
            return 1;
        }
    }
    return -1;
}

int bishop(int sx, int sy, int ex, int ey){
    int bishop_vec[4][2] = {
        {-1, -1},          { 1, -1},

        {-1,  1},          { 1,  1}
    };
    int i, j, dx, dy;
    int player = board[sy][sx] / BISHOP;

    dx = ex - sx;
    dy = ey - ex;

    for(i = 0; i < 4; ++i){
        if((dx%bishop_vec[i][0] == 0) && (dy%bishop_vec[i][1] == 0)){
            j = 1;
            while((sx+bishop_vec[i][0]*j != ex) && (sy+bishop_vec[i][1]*j != ey)){
                if(board[sy+bishop_vec[i][1]*j][sx+bishop_vec[i][0]*j] != 0) return -1;
                j++;
            }
            board[sy][sx] = 0;
            board[ey][ex] = BISHOP * player;
            return 1;
        }
    }
    return -1;
}

int knight(int sx, int sy, int ex, int ey){
    int knight_vec[8][2] = {
                  {-1, -2}, { 1, -2}, 
        {-2, -1},                     { 2, -1},
        {-2,  1},                     { 2,  1},
                  {-1,  2}, { 1,  2}
    };
    int i, j, dx, dy;
    int player = board[sy][sx] / KNIGHT;

    dx = ex - sx;
    dy = ey - ex;

    for(i = 0; i < 8; ++i){
        if((dx == knight_vec[i][0]) && dy == knight_vec[i][1]){
            board[sy][sx] = 0;
            board[ey][ex] = KNIGHT * player;
            return 1;
        }
    }
    return -1;
}
int rook(int sx, int sy, int ex, int ey){
    int rook_vec[4][2] = {
                  { 0, -1}, 
        {-1,  0},           { 1,  0},
                  { 0,  1},
    };
    int i, j, dx, dy;
    int player = board[sy][sx] / KING;

    dx = ex - sx;
    dy = ey - ex;

    for(i = 0; i < 8; ++i){
        if((dx%rook_vec[i][0] == 0) && (dy%rook_vec[i][1])){
            j = 1;
            while((sx+rook_vec[i][0]*j != ex) && (sy+rook_vec[i][1]*j != ey)){
                if(board[sy+rook_vec[i][1]*j][sx+rook_vec[i][0]*j] != 0) return -1;
                j++;
            }
            board[sy][sx] = 0;
            board[ey][ex] = ROOK * player;
            return 1;
        }
    }
    return -1;
}

int pawn(int sx, int sy, int ex, int ey){
    int pawn_vec[4][2] = {
                  { 0, -2},
        {-1, -1}, { 0, -1}, { 1, -1},

    };
    int i, j, dx, dy;
    int player = board[sy][sx] / PAWN;

    dx = ex - sx;
    dy = ey - ex;

    if(player > 0){
        for(i = 0; i < 4; ++i){
            if((dx == pawn_vec[i][0]) && (dy == pawn_vec[i][1])){
                if(i == 0){
                    if(sy != 6) return -1;
                }
                else if((i == 1) || (i == 3)){
                    if(board[ey][ex]*player >= 0) return -1;
                }
                else{
                    if(board[ey][ex]*player < 0) return -1;
                }

                board[sy][sx] = 0;
                if(ey == 0) board[ey][ex] = QUEEN;
                else board[ey][ex] = PAWN;
                return 1;
            }
        }
    }

    else{
        for(i = 0; i < 4; ++i){
            if((dx == pawn_vec[i][0]*(-1)) && (dy == pawn_vec[i][1]*(-1))){
                if(i == 0){
                    if(sy != 1) return -1;
                }
                else if((i == 1) || (i == 3)){
                    if(board[ey][ex]*player >= 0) return -1;
                }
                else{
                    if(board[ey][ex]*player < 0) return -1;
                }

                board[sy][sx] = 0;
                if(ey == 7) board[ey][ex] = QUEEN;
                else board[ey][ex] = PAWN;
                return 1;
            }
        }
    }

    return -1;
}
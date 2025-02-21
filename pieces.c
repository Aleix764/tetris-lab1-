#include "pieces.h"

void init_piece(Piece *p){
    p->name = '-';
    for(int i=0; i<PIECE_SIZE; ++i)
      for(int j=0; j<PIECE_SIZE; ++j)
        p->board[i][j] = '.';
    p->rows = p->cols = 0;
}

void print_piece(Piece p){
    printf("Name: %c; Size: %dx%d\n", p.name, p.rows, p.cols);
    for(int i=0; i<PIECE_SIZE; ++i){
        for(int j=0; j<PIECE_SIZE; ++j){
            printf("%c", p.board[i][j]);
        }
        printf("\n");
    }
}

void rotate_clockwise(Piece *p){
    char temp_board[PIECE_SIZE][PIECE_SIZE];
    //copiem la peça original al nou taulell, amb rotació de 90 graus
    for (int i = 0; i < p->rows; ++i){
        for (int j = 0; j < p->cols; ++j){
           temp_board[j][p->rows - i - 1] = p->board[i][j];
        }
    }
    //Intercanviem dimensions

    int temp = p->rows;
    p->rows = p->cols;
    p->cols = temp;

    //Actualitzem el taulell original
    for(int i = 0; i < PIECE_SIZE; ++i){
        for (int j = 0; j < PIECE_SIZE; ++j){
            if (i < p->rows && j < p->cols){
                p->board[i][j] = temp_board[i][j];
            }else{
                p->board[i][j] = '.'; //Omplim la resta amb punts   
            }
        }
    }


}

void rotate_counter_clockwise(Piece *p){
char temp_board[PIECE_SIZE][PIECE_SIZE];
    //copiem la peça original al nou taulell, amb rotació de 90 graus
    for (int i = 0; i < p->rows; ++i){
        for (int j = 0; j < p->cols; ++j){
           temp_board[p->cols - j - 1][i] = p->board[i][j];
        }
    }
    //Intercanviem dimensions

    int temp = p->rows;
    p->rows = p->cols;
    p->cols = temp;

    //Actualitzem el taulell original
    for(int i = 0; i < PIECE_SIZE; ++i){
        for (int j = 0; j < PIECE_SIZE; ++j){
            if (i < p->rows && j < p->cols){
                p->board[i][j] = temp_board[i][j];
            }else{
                p->board[i][j] = '.'; //Omplim la resta amb punts   
            }
        }
    }

}

Piece make_O(){
    Piece piece;
    init_piece(&piece);
    piece.name = 'O'; piece.rows = piece.cols = 2;
    piece.board[0][0] = '#'; piece.board[0][1] = '#';
    piece.board[1][0] = '#'; piece.board[1][1] = '#';
    return piece;
}

Piece make_I(){
    Piece piece;
    init_piece(&piece);
    piece.name = 'I'; piece.rows = 4; piece.cols = 0;
    piece.board[0][0] = '#'; piece.board[1][0] = '#';
    piece.board[2][0] = '#'; piece.board[3][0] = '#';
    return piece;
}


Piece make_S(){
    Piece piece;
    init_piece(&piece);
    piece.name = 'S'; piece.rows =2; piece.cols = 3;
    piece.board[0][1] = '#'; piece.board[1][0] = '#';
    piece.board[0][2] = '#'; piece.board[1][1] = '#';
    return piece;
}



Piece make_Z(){
    Piece piece;
    init_piece(&piece);
    piece.name = 'Z'; piece.rows =2; piece.cols = 3;
    piece.board[0][0] = '#'; piece.board[0][1] = '#';
    piece.board[1][2] = '#'; piece.board[1][1] = '#';
    return piece;
}

Piece make_L(){
    Piece piece;
    init_piece(&piece);
    piece.name = 'L'; piece.rows = 3; piece.cols = 2;
    piece.board[0][0] = '#'; piece.board[2][0] = '#';
    piece.board[1][0] = '#'; piece.board[2][1] = '#';
    return piece;
}

Piece make_J(){
    Piece piece;
    init_piece(&piece);
    piece.name = 'J'; piece.rows = 3; piece.cols = 2;
    piece.board[2][0] = '#'; piece.board[1][1] = '#';
    piece.board[0][1] = '#'; piece.board[2][1] = '#';
    return piece;
}

Piece make_T(){
    Piece piece;
    init_piece(&piece);
    piece.name = 'T'; piece.rows = 2; piece.cols = 3;
    piece.board[0][0] = '#'; piece.board[0][2] = '#';
    piece.board[0][1] = '#'; piece.board[1][1] = '#';
    return piece;
}

#ifndef __PIECES_H__
#define __PIECES_H__

#include "common.h"

#define MAX_NUM_PIECES 7
#define PIECE_SIZE 4

typedef struct{
	char name;
	char board[PIECE_SIZE][PIECE_SIZE];
	int cols,rows;
} Piece;

// Lab 1 - initialize, print, rotate and make pieces
void init_piece(Piece *p);
void print_piece(Piece p);
void rotate_clockwise(Piece *p);
void rotate_counter_clockwise(Piece *p);

Piece make_O();
Piece make_I();
Piece make_S();
Piece make_Z();
Piece make_L();
Piece make_J();
Piece make_T();

#endif

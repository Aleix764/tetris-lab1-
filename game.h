#ifndef _GAME_H_
#define _GAME_H_

#include "common.h"
#include "utils.h"
#include "pieces.h"

/// ToDo in LAB 1 - define macros MAX_ROWS and MAX_COLUMNS
#define MIN_ROWS 10
#define MIN_COLUMNS 6

typedef struct{
    Piece p;
    int at_row;
    int at_col;
} PieceInfo;

typedef struct{
    int score;
    int rows;
    int columns;
    char** board;
    PieceInfo current_piece;
} GameState;

/// Implemented functions
void print_game_state(GameState *gs);
void print_options();
bool is_valid_option(int option);
void print_line(int columns);
void print_board(GameState *game_state);
void get_new_random_piece(GameState *game_state);
void block_current_piece(GameState *game_state);
bool is_collision(GameState *gs);

/**** LAB 1 - functions to program (start here) ****/
void init_game_state(GameState *game_state);
bool is_terminal(GameState *gs); // True if contains X values in TOP-4 rows
void move_piece(GameState *gs, int option);
void rotate_piece(GameState *gs, int option);
/**** LAB 1 - functions to program (end here) ****/
void make_board(GameState *game_state);
/// Implemented functions
void run_turn(GameState *game_state, int option);
void free_game_state(GameState *game_state);
void set_default_game_state(GameState *game_state);
void restart_game_state(GameState *game_state);
int recursive_best_score(GameState *game_state, int depth);
GameState copy(GameState *game_state);


#endif

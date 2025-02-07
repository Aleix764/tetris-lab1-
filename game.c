#include "game.h"

/**** LAB 1 - given functions ****/
void print_game_state(GameState *gs){
    printf("*** Game State ***\n");
    printf("Score=%d\n", gs->score);
    print_board(gs);
}

void print_options(){
    printf("Options:\n");
    printf("\t%d. Move left\n", MOVE_LEFT);
    printf("\t%d. Move right\n", MOVE_RIGHT);
    printf("\t%d. Rotate clockwise\n", ROTATE_CW);
    printf("\t%d. Rotate counter clockwise\n", ROTATE_CCW);
    printf("\t%d. None\n", NONE);
    printf("\t%d. Show best move\n", SHOW_BEST_MOVE);
    printf("\t%d. Quit game\n", QUIT_GAME);
}

bool is_valid_option(int option){
    return ((MOVE_LEFT <= option) && (option<=QUIT_GAME));
}


void print_line(){
    for(int c=-1; c<columns+1; ++c) 
    	printf("-");    
    printf("\n");
}

void print_board(GameState *game_state){
    Piece *piece = &game_state->current_piece.p;
    int p_row_size = piece->rows;
    int p_col_size = piece->cols;
    int current_row = game_state->current_piece.at_row;
    int current_col = game_state->current_piece.at_col;
    print_line();
    for(int r = 0; r < rows; ++r){
        if(r == 4) print_line();
        printf("|");
        for(int c=0; c < columns; ++c){
            if((game_state->board[r][c] == '.') &&
               (current_row <= r) && (r < current_row + p_row_size) && 
               (current_col <= c) && (c < current_col + p_col_size)){
                printf("%c", piece->board[r-current_row][c-current_col]);
            }
            else printf("%c", game_state->board[r][c]);
        }
        printf("|\n");
    }
    print_line();
    printf("\n");
}

void get_new_random_piece(GameState *game_state){
    // Random piece
    switch (rand()%MAX_NUM_PIECES){
    case 0:
        game_state->current_piece.p = make_I();
        break;    
    case 1:
        game_state->current_piece.p = make_J();
        break;
    case 2:
        game_state->current_piece.p = make_L();
        break;
    case 3:
        game_state->current_piece.p = make_O();
        break;
    case 4:
        game_state->current_piece.p = make_S();
        break;
    case 5:
        game_state->current_piece.p = make_T();
        break;
    case 6:
        game_state->current_piece.p = make_Z();
        break;
    default:
        break;
    }

    // Random orientation
    int rotations = rand()%3;
    for(int r = 0; r < rotations; ++r)
        rotate_clockwise(&(game_state->current_piece.p));

    // Random location
    game_state->current_piece.at_row = 4 - game_state->current_piece.p.rows;
    game_state->current_piece.at_col = rand() % (columns + 1 - game_state->current_piece.p.cols);
}

void block_current_piece(GameState *game_state){
    Piece *piece = &game_state->current_piece.p;
    int row = game_state->current_piece.at_row;
    int col = game_state->current_piece.at_col;
    for(int i=0; i<piece->rows; ++i)
        for(int j=0; j<piece->cols; ++j)
            if(piece->board[i][j] == '#')
                game_state->board[row+i][col+j] = 'X';
}

bool is_collision(char board[rows][columns], PieceInfo *piece_info){
    Piece *piece = &piece_info->p;
    int p_row_size = piece->rows;
    int p_col_size = piece->cols;
    int row = piece_info->at_row;
    int col = piece_info->at_col;

    if((row < 0) || (col < 0) || (row+p_row_size-1 >= rows) || (col+p_col_size-1 >= columns))
    	return true; // piece is out of the grid bounds
    
    for(int i=0; i<piece->rows; ++i)
        for(int j=0; j<piece->cols; ++j)
            if((piece->board[i][j] == '#') && (board[row+i][col+j] == 'X'))
            	return true; // piece collides with another structure in the board
    
    return false;
}

int remove_completed_lines(char board[rows][columns]){
    int lines = 0;
    bool completed_line;
    for(int r=4; r<rows; ++r){
        completed_line = true;
        for(int c=0; c<columns; ++c){
            if(board[r][c] != 'X'){
                completed_line = false; 
                break;
            }
        }
        if(completed_line){
            ++lines;
            // Move all rows above, once down
            for(int r2=r; r2>3; --r2){
                for(int c=0; c<columns; ++c){
                    board[r2][c] = board[r2-1][c];
                }
            }
        }
    }
    return lines;
}

/********************************************************/
/******* LAB 1 - functions to program (start here) ******/
/********************************************************/

void init_game_state(GameState *game_state){

for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            game_state->board[i][j] = '.';
        }
    }
    game_state->score = 0;
    get_new_random_piece(game_state);
}


bool is_terminal(char board[rows][columns]){
      for(int i = 0; i < 4; i++){
        for(int j = 0; j < columns; j++){
            if(board[i][j] == 'X'){
                return true;
            }
        }
    }
    return false;
}


void move_piece(char board[rows][columns], PieceInfo *piece_info, int option){
    
    int new_col = piece_info->at_col + (option == MOVE_RIGHT) - (option == MOVE_LEFT);
    
    PieceInfo temp_piece = *piece_info;
    temp_piece.at_col = new_col;
    
    if (!is_collision(board, &temp_piece)) {
        piece_info->at_col = new_col;
    }
}

void rotate_piece(char board[rows][columns], PieceInfo *piece_info, int option){

    Piece temp_piece = piece_info->p;
    
    if (option == ROTATE_CW) {
        rotate_clockwise(&temp_piece);
    } else if (option == ROTATE_CCW) {
        rotate_counter_clockwise(&temp_piece);
    }
    
    if (!is_collision(board, &(PieceInfo){temp_piece, piece_info->at_row, piece_info->at_col})) {
        piece_info->p = temp_piece;
    }
}
/********************************************************/
/******* LAB 1 - functions to program (end here) ********/
/********************************************************/





void run_turn(GameState *game_state, int option){
	PieceInfo *p_inf = &(game_state->current_piece);
	if(option == MOVE_LEFT || option == MOVE_RIGHT) 
		move_piece(game_state->board, p_inf, option);
	else if(option == ROTATE_CW || option == ROTATE_CCW)
		rotate_piece(game_state->board, p_inf, option);
    else if(option == NONE){} // do nothing
    else{ printf("[ERROR] Invalid option %d.\n", option); exit(-3); }

	// Move down if possible, otherwise block the piece and remove
    // the completed lines, aggregating them to the current score.
    // If it is not in a terminal state, add a new random piece to the board.
	p_inf->at_row++;
	if(is_collision(game_state->board, p_inf)){
		p_inf->at_row--;
		block_current_piece(game_state);
        game_state->score += remove_completed_lines(game_state->board);
        if(!is_terminal(game_state))
            get_new_random_piece(game_state);
	}
}

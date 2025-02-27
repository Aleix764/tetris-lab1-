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


void print_line(int columns){
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
    print_line(game_state->columns);
    for(int r = 0; r < game_state->rows; ++r){
        if(r == 4) print_line(game_state->columns);
        printf("|");
        for(int c=0; c < game_state->columns; ++c){
            if((game_state->board[r][c] == '.') &&
               (current_row <= r) && (r < current_row + p_row_size) && 
               (current_col <= c) && (c < current_col + p_col_size)){
                printf("%c", piece->board[r-current_row][c-current_col]);
            }
            else printf("%c", game_state->board[r][c]);
        }
        printf("|\n");
    }
    print_line(game_state->columns);
    printf("\n");
}

void free_game_state(GameState *game_state){
    if(game_state->board !=NULL){
        for(int r=0; r<game_state->rows; r++){
            free(game_state->board);
        }
    }
	
    free(game_state->board);
    game_state->board=NULL; 
}

void make_board(GameState *game_state){
    if(game_state->board == NULL){
        game_state->board= (char**)malloc(sizeof(char*)*game_state->rows);
        for(int r=0;r<game_state->rows; r++){

            game_state->board[r] = (char*)malloc(sizeof(char)*game_state->columns);
        }
    }
    else{
        game_state->board = (char**)realloc(game_state->board, sizeof(char*)*game_state->rows);
        for(int r=0; r<game_state->rows; r++){
            game_state->board[r] = (char*)malloc(sizeof(char)*game_state->columns);
        }
    }
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
    game_state->current_piece.at_col = rand() % (game_state->columns + 1 - game_state->current_piece.p.cols);
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

bool is_collision(GameState *game_state){
    PieceInfo *piece_info = &(game_state->current_piece);
    Piece *piece = &piece_info->p;
    int p_row_size = piece->rows;
    int p_col_size = piece->cols;
    int row = piece_info->at_row;
    int col = piece_info->at_col;

    if((row < 0) || (col < 0) || (row+p_row_size-1 >= game_state->rows) || (col+p_col_size-1 >= game_state->columns))
    	return true; // piece is out of the grid bounds
    
    for(int i=0; i<piece->rows; ++i)
        for(int j=0; j<piece->cols; ++j)
            if((piece->board[i][j] == '#') && (game_state->board[row+i][col+j] == 'X'))
            	return true; // piece collides with another structure in the board
    
    return false;
}

int remove_completed_lines(GameState *game_state){
    int lines = 0;
    bool completed_line;
    for(int r=4; r<game_state->rows; ++r){
        completed_line = true;
        for(int c=0; c<game_state->columns; ++c){
            if(game_state->board[r][c] != 'X'){
                completed_line = false; 
                break;
            }
        }
        if(completed_line){
            ++lines;
            // Move all rows above, once down
            for(int r2=r; r2>3; --r2){
                for(int c=0; c<game_state->columns; ++c){
                    game_state->board[r2][c] = game_state->board[r2-1][c];
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
    game_state->board = NULL; // board a null
    game_state->rows = MIN_ROWS; 
    game_state->columns = MIN_COLUMNS;
    make_board(game_state); // Assigna memoria al board.
    set_default_game_state(game_state);
}


bool is_terminal(GameState *game_state){
      for(int i = 0; i < 4; i++){
        for(int j = 0; j < game_state->columns; j++){
            if(game_state->board[i][j] == 'X'){
                return true;
            }
        }
    }
    return false;
}


void move_piece(GameState *game_state, int option){
    if (game_state == NULL) {
        return; 
    }
    PieceInfo *piece_info = &(game_state->current_piece);
    int new_col = piece_info->at_col + (option == MOVE_RIGHT) - (option == MOVE_LEFT);
    if (new_col < 0 || new_col + piece_info->p.cols > game_state->columns) {
        return; // No moure la peça si està fora dels limits
    }

    PieceInfo temp_piece = *piece_info;
    temp_piece.at_col = new_col; 
    if (!is_collision(game_state)) {
        piece_info->at_col = new_col; 
    }
}

void rotate_piece(GameState *game_state, int option){
    if (game_state == NULL) {
        return; 
    }

    PieceInfo *piece_info = &(game_state->current_piece);
    Piece temp_piece = piece_info->p;
    if (option == ROTATE_CW) {
        rotate_clockwise(&temp_piece);
    } else if (option == ROTATE_CCW) {
        rotate_counter_clockwise(&temp_piece);
    } else {
        return; 
    }

    if (piece_info->at_col + temp_piece.cols > game_state->columns ||
        piece_info->at_row + temp_piece.rows > game_state->rows) {
        return; // No rotar si anem fora els límits
    }

    PieceInfo temp_piece_info = *piece_info;
    temp_piece_info.p = temp_piece;
    if (!is_collision(game_state)) {
        piece_info->p = temp_piece; 
    }
}
/********************************************************/
/******* LAB 1 - functions to program (end here) ********/
/********************************************************/





bool run_turn(GameState *game_state, int option){
	PieceInfo *p_inf = &(game_state->current_piece);
	if(option == MOVE_LEFT || option == MOVE_RIGHT) 
		move_piece(game_state, option);
	else if(option == ROTATE_CW || option == ROTATE_CCW)
		rotate_piece(game_state, option);
    else if(option == NONE){} // do nothing
    else{ printf("[ERROR] Invalid option %d.\n", option); exit(-3); }

	// Move down if possible, otherwise block the piece and remove
    // the completed lines, aggregating them to the current score.
    // If it is not in a terminal state, add a new random piece to the board.
    bool last_move=false;
    p_inf->at_row++;
	if(is_collision(game_state)){
        return true;
		p_inf->at_row--;
		block_current_piece(game_state);
        game_state->score += remove_completed_lines(game_state);
        if(!is_terminal(game_state))
            get_new_random_piece(game_state);
	}

    return last_move;
}


void set_default_game_state(GameState *game_state){
    game_state->score=0;

    for (int r = 0; r < game_state->rows; ++r) {
            for (int c = 0; c < game_state->columns; ++c) {
                game_state->board[r][c] = '.';
            }
        }
    
        // Obtenir nova peça
        get_new_random_piece(game_state);
    }


void restart_game_state(GameState *game_state) {

    printf("Enter the number of rows (minimum %d): ", MIN_ROWS);
    game_state->rows = read_int();
    while (game_state->rows < MIN_ROWS) {
        printf("Rows must be at least %d. Enter again: ", MIN_ROWS);
        game_state->rows = read_int();
    }
    
    printf("Enter the number of columns (minimum %d): ", MIN_COLUMNS);
    game_state->columns = read_int();
    while (game_state->columns < MIN_COLUMNS) {
        printf("Columns must be at least %d. Enter again: ", MIN_COLUMNS);
        game_state->columns = read_int();
    }

    make_board(game_state);
    set_default_game_state(game_state);
    }

GameState copy(GameState *game_state){
    GameState copy;
    if(game_state==NULL){ //if we have an invalid state we return it null or at 0
        copy.board=NULL;
        copy.score=0;
        copy.rows=0;
        copy.columns=0;
        return copy;
    }

    copy.score=game_state->score;
    copy.rows=game_state->rows;
    copy.columns=game_state->columns;
    copy.current_piece=game_state->current_piece; //copy actual piece

    copy.board=(char**)malloc(copy.rows *(sizeof(char))); //copy board dynamic
    for(int i=0; i<copy.rows; i++){
        copy.board[i]=(char *)malloc(copy.columns *(sizeof(char)));
        for(int j=0; j<copy.columns; j++){
            copy.board[i][j]=game_state->board[i][j];
        }
    } 

    return copy;

}


int recursive_best_score(GameState *game_state, int depth){
    if(depth>=MAX_DEPTH || is_terminal(game_state)){
        return game_state->score;
    }
    int options[]={MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE}; 
    int score=game_state->score;
    int best_score=score;
    for(int i =0; i<sizeof(options); i++){
        GameState copied_state= copy(game_state);
        run_turn(&copied_state, options[i]); //iterate through all the options 
        int current_score = recursive_best_score(&copied_state, depth + 1);
        if (current_score > best_score) { //change best score if needed
            best_score = current_score;
        }

        // Free memory ocupied by the copy
        free_game_state(&copied_state);
    }

    return best_score;
}

int show_best_move(GameState *game_state){
    int best_score=game_state->score;
    int best_move=NONE;
    int options[5]={MOVE_LEFT, MOVE_RIGHT, ROTATE_CW, ROTATE_CCW, NONE};
    int num_options=sizeof(options);
    for(int i=0; i<num_options; i++){
        GameState copied_state= copy(game_state);
                // turn on copy with actual option
                run_turn(&copied_state, options[i]);

                // check score of the actual turn
                int current_score = recursive_best_score(&copied_state, 0);
        
                // change if actual score is higher
                if (current_score > best_score) {
                    best_score = current_score;
                    best_move = options[i];
                }
        
                // free memory
                free_game_state(&copied_state);
            }
        
            // return the move
            return best_move;
        }

#include "main.h"

void run_game(Session *session){
    GameState *game_state = &session->current_game_state;

    bool finish_game; 

    do{
        // 1. Print data
        printf("Best score: %d\n", session->best_score);
        print_game_state(game_state);

        // 2. Select a move
        int game_option;
        do{
            print_options(game_state->board);
            printf("Enter a game option: ");
            game_option = read_int();
        }while(!is_valid_option(game_option));
        
        // 3.a Quit game
        if(game_option == QUIT_GAME)
            return;
        // 3.b Show best move
        if(game_option == SHOW_BEST_MOVE){
            // ToDo in Lab 3
            continue;
        }

        // 3.c Run one turn with the given option
        run_turn(game_state, game_option);

        // 4. Update scores
        session->best_score = myMax(session->best_score, game_state->score);
        
        // 5. Add new number
        finish_game = is_terminal(game_state);
    }while(!finish_game);

    printf("*** GAME OVER ***\n");
    print_game_state(game_state);
}

void new_game(Session *session){
    restart_session_game_state(session);
    run_game(session);
}

void save_game(Session *session){
    // ToDo in Lab 2
}

void load_game(Session *session){
    // ToDo in Lab 2
}

void resume_game(Session *session){
    if(!is_terminal(&(session->current_game_state))){
        run_game(session); //si no es detecta que sigui terminal, continuar on erem
    }
    else{
        printf("Game state is terminal. Cannot be resumed \n");
    }
    // ToDo in Lab 2
}

void print_menu(){
    printf("Menu options:\n");
    printf("\t1. New game.\n");  // LAB1 - basic lab for creating board, pieces and movements
                                 // LAB1 - extend game with GameState data structure, current score, best score
                                 //      - also add standard (4x4) with a posible extension, read board from command line  
    printf("\t2. Save game.\n"); // LAB2 - Writing file
    printf("\t3. Load game.\n"); // LAB2 - Reading file
    printf("\t4. Resume game.\n"); // LAB2 - Continue game after reading file
    printf("\t5. Exit.\n");  // Free all reserved memory!
}


void run(Session *session){
    int option;
    do{
        print_menu();
        do{
            printf("Enter an integer (%d-%d): ", NEW_GAME, EXIT);
            option = read_int();
        }while(option < NEW_GAME || option > EXIT);

        switch (option){
        case NEW_GAME:
            new_game(session);
            break;
        case SAVE_GAME:
            save_game(session);
            break;
        case LOAD_GAME:
            load_game(session);
            break;
        case RESUME_GAME:
            resume_game(session);
            break;
        case EXIT:
            free_game_state(&(session->current_game_state));
            break;
        }
    }while(option != EXIT);
}

void save_game(Session *session) {
    char filename[100];
    printf("Enter filename to save game: ");
    scanf("%99s", filename);  // Read filename from user

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        return;
    }

    GameState *game = &session->current_game_state;
    PieceInfo *piece = &game->current_piece;

    // Write score
    fprintf(file, "Score: %d\n\n", game->score);

    // Write PieceInfo
    fprintf(file, "PieceInfo:\n");
    fprintf(file, "at_row: %d\n", piece->at_row);
    fprintf(file, "at_col: %d\n", piece->at_col);
    fprintf(file, "name: %c\n", piece->p.name);
    fprintf(file, "rows: %d\n", piece->p.rows);
    fprintf(file, "cols: %d\n", piece->p.cols);

    // Print piece shape
    for (int i = 0; i < piece->p.rows; i++) {
        for (int j = 0; j < piece->p.cols; j++) {
            fprintf(file, "%c", piece->p.board[i][j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n");

    // Write board info
    fprintf(file, "Board:\n");
    fprintf(file, "rows: %d\n", game->rows);
    fprintf(file, "cols: %d\n", game->columns);

    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->columns; j++) {
            char cell = game->board[i][j];

            // Ensure we don’t print the current piece on the board
            if (i >= piece->at_row && i < piece->at_row + piece->p.rows &&
                j >= piece->at_col && j < piece->at_col + piece->p.cols) {
                if (piece->p.board[i - piece->at_row][j - piece->at_col] != '.') {
                    cell = '.';  // Replace piece cells with empty space
                }
            }

            fprintf(file, "%c", cell);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Game saved successfully to %s\n", filename);
}

void load_game(Session *session) {
    char filename[100];
    printf("Enter filename to load game: ");
    scanf("%99s", filename);  // Read filename from user

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    GameState *game = &session->current_game_state;
    PieceInfo *piece = &game->current_piece;

    // Read score
    fscanf(file, "Score: %d\n\n", &game->score);

    // Read PieceInfo
    fscanf(file, "PieceInfo:\n");
    fscanf(file, "at_row: %d\n", &piece->at_row);
    fscanf(file, "at_col: %d\n", &piece->at_col);
    fscanf(file, "name: %c\n", &piece->p.name);
    fscanf(file, "rows: %d\n", &piece->p.rows);
    fscanf(file, "cols: %d\n", &piece->p.cols);

    // Read the piece shape
    for (int i = 0; i < piece->p.rows; i++) {
        fscanf(file, "%s", piece->p.board[i]);  // Read row as string
    }

    // Read board dimensions
    fscanf(file, "\nBoard:\n");
    fscanf(file, "rows: %d\n", &game->rows);
    fscanf(file, "cols: %d\n", &game->columns);

    // Allocate board memory
    make_board(game);

    // Read the board content
    for (int i = 0; i < game->rows; i++) {
        fscanf(file, "%s", game->board[i]);
    }

    fclose(file);
    printf("Game loaded successfully from %s\n", filename);
}

int main(){
    Session session;
    init_session(&session);
    run(&session);
}

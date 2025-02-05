#ifndef _MAIN_H_
#define _MAIN_H_

#include "common.h"
#include "utils.h"
#include "game.h"
#include "session.h"

void run_game(Session *session);

void new_game(Session *session);

void save_game(Session *session);

void load_game(Session *session);

void resume_game(Session *session);

void print_menu();

void run(Session *session);

#endif 

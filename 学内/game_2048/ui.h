#ifndef UI_H
#define UI_H

#include "game.h"

void init_ui();
void draw_board(int **board, int size, int score);
void draw_game_over(void);
void end_ui();
void set_congrats(int enabled);
int get_congrats();

#endif

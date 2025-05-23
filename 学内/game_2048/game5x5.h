#pragma once

#define SIZE5 5

void init_board5(int board[SIZE5][SIZE5]);
void draw_board5(int board[SIZE5][SIZE5], int score);
int get_score5(int board[SIZE5][SIZE5]);
int move_left5(int board[SIZE5][SIZE5]);
int move_right5(int board[SIZE5][SIZE5]);
int move_up5(int board[SIZE5][SIZE5]);
int move_down5(int board[SIZE5][SIZE5]);
void add_random_tile5(int board[SIZE5][SIZE5]);
int is_game_over5(int board[SIZE5][SIZE5]);

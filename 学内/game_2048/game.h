#ifndef GAME_H
#define GAME_H

#define SIZE 4

// ゲーム初期化
void init_board(int board[SIZE][SIZE]);

// タイルをランダムに1つ追加（成功:1, 失敗:0）
int add_random_tile(int board[SIZE][SIZE]);

// 移動処理（移動または合体があれば 1, なければ 0）
int move_left(int board[SIZE][SIZE]);
int move_right(int board[SIZE][SIZE]);
int move_up(int board[SIZE][SIZE]);
int move_down(int board[SIZE][SIZE]);

// スコア取得
int get_score(int board[SIZE][SIZE]);

// ゲームオーバー判定（true: 終了）
int is_game_over(int board[SIZE][SIZE]);

#endif

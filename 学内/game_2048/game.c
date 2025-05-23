#include <stdlib.h>
#include <string.h>
#include "game.h"

#define PROBABILITY_OF_4 10  // 10% の確率で 4 を生成

static int score = 0;

void init_board(int board[SIZE][SIZE]) {
    memset(board, 0, sizeof(int) * SIZE * SIZE);
    add_random_tile(board);
    add_random_tile(board);
    score = 0;
}

int get_score(int board[SIZE][SIZE]) {
    return score;
}

int add_random_tile(int board[SIZE][SIZE]) {
    int empty[SIZE * SIZE][2];
    int count = 0;

    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if (board[y][x] == 0) {
                empty[count][0] = y;
                empty[count][1] = x;
                count++;
            }
        }
    }

    if (count == 0) return 0;

    int r = rand() % count;
    int y = empty[r][0];
    int x = empty[r][1];
    board[y][x] = (rand() % 100 < PROBABILITY_OF_4) ? 4 : 2;

    return 1;
}

static int slide_and_merge_row(int row[SIZE]) {
    int moved = 0;
    int tmp[SIZE] = {0};
    int idx = 0;

    // スライド（左詰め）
    for (int i = 0; i < SIZE; i++) {
        if (row[i] != 0) {
            tmp[idx++] = row[i];
        }
    }

    // マージ
    for (int i = 0; i < SIZE - 1; i++) {
        if (tmp[i] != 0 && tmp[i] == tmp[i + 1]) {
            tmp[i] *= 2;
            tmp[i + 1] = 0;
            score += tmp[i];
            moved = 1;
        }
    }

    // 再スライド
    int final[SIZE] = {0};
    idx = 0;
    for (int i = 0; i < SIZE; i++) {
        if (tmp[i] != 0) {
            final[idx++] = tmp[i];
        }
    }

    // 元と比較して変化があったかを確認
    for (int i = 0; i < SIZE; i++) {
        if (row[i] != final[i]) {
            row[i] = final[i];
            moved = 1;
        }
    }

    return moved;
}

int move_left(int board[SIZE][SIZE]) {
    int moved = 0;
    for (int y = 0; y < SIZE; y++) {
        if (slide_and_merge_row(board[y])) {
            moved = 1;
        }
    }
    return moved;
}

int move_right(int board[SIZE][SIZE]) {
    int moved = 0;
    for (int y = 0; y < SIZE; y++) {
        int reversed[SIZE];
        for (int i = 0; i < SIZE; i++) {
            reversed[i] = board[y][SIZE - 1 - i];
        }
        if (slide_and_merge_row(reversed)) {
            moved = 1;
        }
        for (int i = 0; i < SIZE; i++) {
            board[y][SIZE - 1 - i] = reversed[i];
        }
    }
    return moved;
}

int move_up(int board[SIZE][SIZE]) {
    int moved = 0;
    for (int x = 0; x < SIZE; x++) {
        int col[SIZE];
        for (int y = 0; y < SIZE; y++) {
            col[y] = board[y][x];
        }
        if (slide_and_merge_row(col)) {
            moved = 1;
        }
        for (int y = 0; y < SIZE; y++) {
            board[y][x] = col[y];
        }
    }
    return moved;
}

int move_down(int board[SIZE][SIZE]) {
    int moved = 0;
    for (int x = 0; x < SIZE; x++) {
        int col[SIZE];
        for (int y = 0; y < SIZE; y++) {
            col[y] = board[SIZE - 1 - y][x];
        }
        if (slide_and_merge_row(col)) {
            moved = 1;
        }
        for (int y = 0; y < SIZE; y++) {
            board[SIZE - 1 - y][x] = col[y];
        }
    }
    return moved;
}

int is_game_over(int board[SIZE][SIZE]) {
    int copy[SIZE][SIZE];
    memcpy(copy, board, sizeof(int) * SIZE * SIZE);

    if (move_left(copy)) return 0;
    if (move_right(copy)) return 0;
    if (move_up(copy)) return 0;
    if (move_down(copy)) return 0;

    return 1;
}

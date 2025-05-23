#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "game5x5.h"

void init_board5(int board[SIZE5][SIZE5]) {
    for (int y = 0; y < SIZE5; y++)
        for (int x = 0; x < SIZE5; x++)
            board[y][x] = 0;
    add_random_tile5(board);
    add_random_tile5(board);
}

int get_score5(int board[SIZE5][SIZE5]) {
    int score = 0;
    for (int y = 0; y < SIZE5; y++)
        for (int x = 0; x < SIZE5; x++)
            score += board[y][x];
    return score;
}

int move_left5(int board[SIZE5][SIZE5]) {
    int moved = 0;
    for (int y = 0; y < SIZE5; y++) {
        int last = -1, last_val = 0, idx = 0;
        int tmp[SIZE5] = {0};
        for (int x = 0; x < SIZE5; x++) {
            if (board[y][x] == 0) continue;
            if (last_val == board[y][x]) {
                tmp[last] *= 2;
                last_val = 0;
                moved = 1;
            } else {
                tmp[idx++] = board[y][x];
                last = idx - 1;
                last_val = board[y][x];
            }
        }
        for (int x = 0; x < SIZE5; x++) {
            if (board[y][x] != tmp[x]) moved = 1;
            board[y][x] = tmp[x];
        }
    }
    return moved;
}

int move_right5(int board[SIZE5][SIZE5]) {
    int moved = 0;
    for (int y = 0; y < SIZE5; y++) {
        int last = SIZE5, last_val = 0, idx = SIZE5 - 1;
        int tmp[SIZE5] = {0};
        for (int x = SIZE5 - 1; x >= 0; x--) {
            if (board[y][x] == 0) continue;
            if (last_val == board[y][x]) {
                tmp[last] *= 2;
                last_val = 0;
                moved = 1;
            } else {
                tmp[idx--] = board[y][x];
                last = idx + 1;
                last_val = board[y][x];
            }
        }
        for (int x = 0; x < SIZE5; x++) {
            if (board[y][x] != tmp[x]) moved = 1;
            board[y][x] = tmp[x];
        }
    }
    return moved;
}

int move_up5(int board[SIZE5][SIZE5]) {
    int moved = 0;
    for (int x = 0; x < SIZE5; x++) {
        int last = -1, last_val = 0, idx = 0;
        int tmp[SIZE5] = {0};
        for (int y = 0; y < SIZE5; y++) {
            if (board[y][x] == 0) continue;
            if (last_val == board[y][x]) {
                tmp[last] *= 2;
                last_val = 0;
                moved = 1;
            } else {
                tmp[idx++] = board[y][x];
                last = idx - 1;
                last_val = board[y][x];
            }
        }
        for (int y = 0; y < SIZE5; y++) {
            if (board[y][x] != tmp[y]) moved = 1;
            board[y][x] = tmp[y];
        }
    }
    return moved;
}

int move_down5(int board[SIZE5][SIZE5]) {
    int moved = 0;
    for (int x = 0; x < SIZE5; x++) {
        int last = SIZE5, last_val = 0, idx = SIZE5 - 1;
        int tmp[SIZE5] = {0};
        for (int y = SIZE5 - 1; y >= 0; y--) {
            if (board[y][x] == 0) continue;
            if (last_val == board[y][x]) {
                tmp[last] *= 2;
                last_val = 0;
                moved = 1;
            } else {
                tmp[idx--] = board[y][x];
                last = idx + 1;
                last_val = board[y][x];
            }
        }
        for (int y = 0; y < SIZE5; y++) {
            if (board[y][x] != tmp[y]) moved = 1;
            board[y][x] = tmp[y];
        }
    }
    return moved;
}

void add_random_tile5(int board[SIZE5][SIZE5]) {
    int empty[SIZE5 * SIZE5][2];
    int n = 0;
    for (int y = 0; y < SIZE5; y++)
        for (int x = 0; x < SIZE5; x++)
            if (board[y][x] == 0) {
                empty[n][0] = y;
                empty[n][1] = x;
                n++;
            }
    if (n > 0) {
        int r = rand() % n;
        board[empty[r][0]][empty[r][1]] = (rand() % 10 == 0) ? 4 : 2;
    }
}

int is_game_over5(int board[SIZE5][SIZE5]) {
    for (int y = 0; y < SIZE5; y++)
        for (int x = 0; x < SIZE5; x++)
            if (board[y][x] == 0)
                return 0;
    for (int y = 0; y < SIZE5; y++)
        for (int x = 0; x < SIZE5 - 1; x++)
            if (board[y][x] == board[y][x + 1])
                return 0;
    for (int x = 0; x < SIZE5; x++)
        for (int y = 0; y < SIZE5 - 1; y++)
            if (board[y][x] == board[y + 1][x])
                return 0;
    return 1;
}

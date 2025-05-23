#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "game.h"
#include "ui.h"
#include "game5x5.h"

int main() {
    int size = 4;
    int ch;

    printf("盤面サイズを選んでください (4: 4x4, 5: 5x5): ");
    scanf("%d", &size);
    if (size != 4 && size != 5) size = 4;

    srand(time(NULL));
    init_ui();

    if (size == 4) {
        int board[SIZE][SIZE];
        init_board(board);
        // ポインタ配列に変換
        int *bptr[SIZE];
        for (int i = 0; i < SIZE; i++) bptr[i] = board[i];
        draw_board(bptr, SIZE, get_score(board));
        while (1) {
            // ===== 4096 の検出と削除 =====
            int found_4096 = 0;
            for (int y = 0; y < SIZE; y++) {
                for (int x = 0; x < SIZE; x++) {
                    if (board[y][x] == 4096) {
                        found_4096 = 1;
                        board[y][x] = 0;  // 4096 を削除
                    }
                }
            }
            if (found_4096) {
                set_congrats(1);  // 「素晴らしい！」表示モードON
            }

            draw_board(bptr, SIZE, get_score(board));

            ch = getch();

            // ===== 「素晴らしい！」メッセージの解除 =====
            if (get_congrats()) {
                set_congrats(0);
                draw_board(bptr, SIZE, get_score(board));  // 再描画で非表示
                continue;
            }

            int moved = 0;

            switch (ch) {
                case KEY_LEFT:  moved = move_left(board);  break;
                case KEY_RIGHT: moved = move_right(board); break;
                case KEY_UP:    moved = move_up(board);    break;
                case KEY_DOWN:  moved = move_down(board);  break;
                case 'q':
                case 'Q':
                    end_ui();
                    printf("Game quit.\n");
                    return 0;
            }

            if (moved) {
                add_random_tile(board);
                draw_board(bptr, SIZE, get_score(board));

                if (is_game_over(board)) {
                    draw_game_over();
                    while ((ch = getch()) != 'q' && ch != 'Q') {}
                    break;
                }
            }
        }

        end_ui();
        return 0;
    } else {
        int board5[SIZE5][SIZE5];
        init_board5(board5);
        int *bptr5[SIZE5];
        for (int i = 0; i < SIZE5; i++) bptr5[i] = board5[i];
        draw_board(bptr5, SIZE5, get_score5(board5));
        while (1) {
            // ===== 4096 の検出と削除 =====
            int found_4096 = 0;
            for (int y = 0; y < SIZE5; y++) {
                for (int x = 0; x < SIZE5; x++) {
                    if (board5[y][x] == 4096) {
                        found_4096 = 1;
                        board5[y][x] = 0;
                    }
                }
            }
            if (found_4096) {
                set_congrats(1);
            }
            draw_board(bptr5, SIZE5, get_score5(board5));
            ch = getch();
            if (get_congrats()) {
                set_congrats(0);
                draw_board(bptr5, SIZE5, get_score5(board5));
                continue;
            }
            int moved = 0;
            switch (ch) {
                case KEY_LEFT:  moved = move_left5(board5);  break;
                case KEY_RIGHT: moved = move_right5(board5); break;
                case KEY_UP:    moved = move_up5(board5);    break;
                case KEY_DOWN:  moved = move_down5(board5);  break;
                case 'q':
                case 'Q':
                    end_ui();
                    printf("Game quit.\n");
                    return 0;
            }
            if (moved) {
                add_random_tile5(board5);
                draw_board(bptr5, SIZE5, get_score5(board5));
                if (is_game_over5(board5)) {
                    draw_game_over();
                    while ((ch = getch()) != 'q' && ch != 'Q') {}
                    break;
                }
            }
        }
        end_ui();
        return 0;
    }
}

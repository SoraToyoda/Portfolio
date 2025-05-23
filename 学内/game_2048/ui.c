#include <locale.h>
#include <ncurses.h>
#include <string.h>
#include "ui.h"

static int show_congrats = 0;

void set_congrats(int enabled) {
    show_congrats = enabled;
}

int get_congrats() {
    return show_congrats;
}

// 色ペアの初期化（init_ui内で呼ぶ）
static void init_colors() {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_RED, COLOR_BLACK);
    // 必要に応じて色を追加
}

void init_ui() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    init_colors();
}


// 数値に応じた色ペアを返す
static int get_color(int value) {
    switch (value) {
        case 2: return 1;
        case 4: return 2;
        case 8: return 3;
        case 16: return 4;
        case 32: return 5;
        case 64: return 6;
        default: return 7;
    }
}

void draw_board(int **board, int size, int score) {
    clear();
    mvprintw(0, 0, "Score: %d", score);

    // 枠線描画（横線）
    for (int y = 0; y <= size; y++) {
        move(2 + y * 2, 0);
        for (int x = 0; x < size; x++) {
            printw("+-----");
        }
        printw("+");
    }

    // 数字と縦線描画
    for (int y = 0; y < size; y++) {
        move(3 + y * 2, 0);
        for (int x = 0; x < size; x++) {
            printw("|");
            int val = board[y][x];
            int color = get_color(val);
            char buf[6];
            if (val != 0) {
                // 中央揃え
                snprintf(buf, sizeof(buf), "%5d", val);
                int len = strlen(buf);
                int left = (5 - len) / 2;
                int right = 5 - len - left;
                attron(COLOR_PAIR(color));
                for (int i = 0; i < left; i++) printw(" ");
                printw("%s", buf + (5 - len));
                for (int i = 0; i < right; i++) printw(" ");
                attroff(COLOR_PAIR(color));
            } else {
                printw("     ");
            }
        }
        printw("|");
    }

    mvprintw(3 + size * 2, 0, "矢印キーで移動、qで終了");
    refresh();
}

void draw_game_over() {
    attron(COLOR_PAIR(8));
    mvprintw(SIZE * 2 + 5, 0, "===== GAME OVER =====");
    attroff(COLOR_PAIR(8));
    mvprintw(SIZE * 2 + 6, 0, "Press 'q' to quit...");
    refresh();
}

void end_ui() {
    endwin();
}

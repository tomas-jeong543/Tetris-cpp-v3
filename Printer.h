#pragma once
#include "Color.h"
#include "Block.h"

class Printer {
    private:
        int ab_x;
        int ab_y;
    public:
        Printer();
        int gotoxy(int x, int y);
        void SetColor(int color);
        void show_keys();
        int show_cur_block(Block& block);
        int erase_cur_block(Block& block);
        int show_next_block(Block& block, int level);
        int show_logo();
        int show_total_block(char total_block[21][14], int level);
        int show_gamestat(int level, int score, int lines_left);
        int show_clear_screen();
        int show_gameover();
};
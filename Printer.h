#pragma once
#include "Color.h"
#include "Block.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>

class Printer {
private:
    int ab_x;
    int ab_y;
public:
    Printer();
    int get_x();
    int get_y();
    int gotoxy(int x, int y);
    void SetColor(int color);
    void show_keys();
    int show_cur_block(Block& block);
    int erase_cur_block(Block& block );
    int show_next_block(Block& block, int level);
    int show_logo();
    //일부 내용이 변경된 함수입니다 cpp파일에 설명이 적혀 있습니다
    int show_total_block(char total_block[21][14], int level);
    int show_gamestat(int level, int score, int lines_left);
    int show_clear_screen(int score);
    int show_gameover();
};

#pragma once
#include "Stages.h"
#include "Block.h"
#include "Printer.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>

class Game {
    private:
        int level;
        int score;
        int lines;
        int i;
	    int is_gameover=0;
        char keytemp;
        char total_block[21][14];
        Stages* stages;
        Printer* printer;
        Block* curr_block;
        Block* next_block;
        Block* keeped_block;
    public:
        Game();
        void run();
        int init();
        int input_data();
        int move_block();
        int strike_check();
        int merge_block();
        int check_full_line();
        int rotate();
        int keep();
};
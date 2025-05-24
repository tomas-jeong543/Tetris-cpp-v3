#pragma once
#include "cstdlib"
#include <time.h>

class Block {
    private:
        static char block[7][4][4][4];
        int block_shape;
        int block_angle;
        int block_x;
        int block_y;
    public:
        Block() = delete;
        Block(int stick_rate);
        int get_x();
        int get_y();
        int get_shape();
        int get_angle();
        char (*get_block())[4];
        int get_number(int i, int j);
        void rotate();
        void rotate(int angle);
        void movetoxy(int x, int y);
        void move_left();
        void move_right();
        void move_down();
        void move_up();
        int start();
};
#pragma once
#include "Stage.h"

class Stages {
    private:
        STAGE stage_data[10];
    public:
        Stages();
        int get_clear_line(int level);
        int get_stick_rate(int level);
};
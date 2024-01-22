//
// Created by loochis on 15/01/24.
//

#include <lib/asciichis/asciichis.h>
#include <lib/mathchis/mathchis.h>

#include <stdio.h>
#include <iostream>
#include <thread>
#include <ncurses.h>
#include <math.h>

using namespace ASCIICHIS;
using namespace MATHCHIS;

#define FIELD_OF_VIEW 100.0

asciirenderer rend;
int testCol;

std::thread *threads;
vector *ray_origins;
vector *ray_dirs;

matrix view_matrix = matrix(3,3);
matrix rotate_matrix = matrix(3,3);
matrix transl_matrix = matrix(3,3);

vector player_pos = vector(0, 0, 1);
vector player_dir = vector(0, 1, 0);

// FIX FLOAT MODULO FOR NEGATIVE NUMBERS
float fixed_mod(float a, float N) {return a - N*floor(a/N);} //return in range [0, N)

void castline(int line) {

    vector cast_dir = ray_dirs[line] * rotate_matrix;
    vector cast_pos = ray_origins[line] * rotate_matrix + player_pos;
    int hits = 0;
    float distance = 0;

    bool x_positive = cast_dir.elems[0] > 0;
    bool y_positive = cast_dir.elems[1] > 0;

    vector col = vector(0);

    for (int i = 0; i < 100; i++) {
        float x_dist = -fixed_mod(cast_pos.elems[0], 1.0f);
        float y_dist = -fixed_mod(cast_pos.elems[1], 1.0f);

        if (x_positive) x_dist = 1+x_dist;
        if (y_positive) y_dist = 1+y_dist;

        float t_x = x_dist / cast_dir.elems[0];
        float t_y = y_dist / cast_dir.elems[1];

        if (t_x < t_y) {
            col.elems[0] = 1; col.elems[1] = 0;
        } else {
            col.elems[0] = 0; col.elems[1] = 1;
        }

        float t = fmin(t_x, t_y);

        float delta_x = cast_dir.elems[0]*(t+0.002f);
        float delta_y = cast_dir.elems[1]*(t+0.002f);

        distance += fsqrt(pow(delta_x, 2) + pow(delta_y, 2));
        cast_pos.elems[0] += delta_x;
        cast_pos.elems[1] += delta_y;

        hits++;

        if (distance > 2)
            break;
    }

    float max = (float)rend.height/2;
    max /= (distance);

    for (int i = 0; i < rend.height; i++) {
        int m_i = i - rend.height/2;
        if (m_i < -max || m_i > max)
            rend.setpix(line, i, vector(0).col_tobytes());
        else
            rend.setpix(line, i, col.col_tobytes());
    }
}

void set_rotation(double angle) {
    double s_a = sin(angle);
    double c_a = cos(angle);
    rotate_matrix.elems[0] = c_a;
    rotate_matrix.elems[1] = s_a;
    rotate_matrix.elems[3] = -s_a;
    rotate_matrix.elems[4] = c_a;
}

void set_translation(vector pos) {
    transl_matrix.elems[6] = pos.elems[0];
    transl_matrix.elems[7] = pos.elems[1];
}

int kbhit()
{
    int ch = getch();

    switch (ch) {
        case ('w'):
            return 1;
        case ('s'):
            return 2;
        case ('a'):
            return 3;
        case ('d'):
            return 4;
        default:
            return 0;
    }
}

void game_loop() {
    float angle = 0;

    while (true) {
        int kb = kbhit();
        switch (kb) {
            case(1):
                player_pos += player_dir*0.2;
                break;
            case(2):
                player_pos -= player_dir*0.2;
                break;
            case(3):
                angle += 0.1;
                break;
            case(4):
                angle -= 0.1;
                break;
            default:
                break;
        }
        if (kb) refresh();

        set_rotation(angle);
        player_dir.elems[0] = -sin(angle);
        player_dir.elems[1] = cos(angle);

        for (int i = 0; i < rend.width; i++) {
            threads[i] = std::thread(castline, i);
        }

        for (int i = 0; i < rend.width; i++) {
            threads[i].join();
        }

        //rend.flushscreen();
        rend.pushscreen();
    }
}

int main () {
    initscr();

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    //scrollok(stdscr, TRUE);

    rend = asciirenderer();
    threads = (std::thread*)malloc(rend.width * sizeof(std::thread));
    ray_dirs = (vector*)malloc(rend.width * sizeof(vector));
    ray_origins = (vector*)malloc(rend.width * sizeof(vector));

    std::vector<float> rotate_vec {1, 1, 0, 1, 1, 0, 0, 0, 1};
    std::vector<float> transl_vec {1, 0, 0, 0, 1, 0, 0, 0, 1};

    rotate_matrix.elems = rotate_vec;
    transl_matrix.elems = transl_vec;

    // Precompute ray directions (use the transform matrix to change view dir)
    for (int i = 0; i < rend.width; i++) {
        double dir_angle = (FIELD_OF_VIEW * (M_PI / 180.0)) * ((i - (rend.width / 2.0)) / rend.width);
        ray_dirs[i] = vector(sin(dir_angle), cos(dir_angle), 0);
        ray_origins[i] = ray_dirs[i] / vector::dot(ray_dirs[i], vector(0, 1, 0));
        ray_dirs[i].elems[2] = 1;
        ray_origins[i].elems[2] = 1;
    }

    set_translation(player_pos);

    game_loop();

    double angle = 0;

    free(ray_dirs);
    free(ray_origins);
    free(threads);

    exit(0);
}
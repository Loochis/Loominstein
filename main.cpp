//
// Created by loochis on 15/01/24.
//

#include <lib/asciichis/asciichis.h>
#include <lib/mathchis/mathchis.h>
#include "lib/texturechis/texture.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <ncurses.h>
#include <math.h>
#include <string.h>

using namespace ASCIICHIS;
using namespace MATHCHIS;

#define FIELD_OF_VIEW 90.0

#define X_POS_LIGHT 1
#define X_NEG_LIGHT 0.6
#define Y_POS_LIGHT 0.8
#define Y_NEG_LIGHT 0.4

asciirenderer rend;

std::thread *threads;
vector *ray_dirs;
float *fisheye_factor;

matrix view_matrix = matrix(3,3);
matrix rotate_matrix = matrix(3,3);
matrix transl_matrix = matrix(3,3);

vector player_pos = vector(0.5, 0.5, 1);
vector player_dir = vector(0, 1, 0);

vector colors[] = {vector(0,0,0),vector(1,1,1), vector(0.5,1,1), vector(1,0.2,0.2)};

int map_width = 0;
int map_height = 0;
int* map;

// TEXTURES
texture tex0 = texture("textures/cobble.bin");

// FIX FLOAT MODULO FOR NEGATIVE NUMBERS
float fixed_mod(float a, float N) {
    return a - N*floor(a/N);
} //return in range [0, N)

int* loadmap(std::string map_path) {
    free(map);

    std::ifstream mapfile (map_path);
    if (!mapfile.is_open()) exit(1);

    std::string line_wh;
    std::string line_sp;
    getline(mapfile, line_wh);
    getline(mapfile, line_sp);

    int decimal_coef = 1;
    int line_w=0, line_h=0, start_x=0, start_y=0;

    // Convert string representation of decimal into decimal number
    for (int i = 3; i >= 0; i--) {
        line_w  += decimal_coef*(line_wh[i]   - 48);
        line_h  += decimal_coef*(line_wh[i+5] - 48);
        start_x += decimal_coef*(line_sp[i]   - 48);
        start_y += decimal_coef*(line_sp[i+5] - 48);
        decimal_coef *= 10;
    }

    map_width  = line_w;
    map_height = line_h;
    player_pos.elems[0] = start_x;
    player_pos.elems[1] = start_y;
    map = (int*)malloc(line_w * line_h * sizeof(int));

    // fill contents of map
    // ROW MAJOR MAP CONTENTS
    std::string map_line_str;
    int map_idx = 0;
    for (int y = 0; y < map_height; y++) {
        getline(mapfile, map_line_str);
        for (int x = 0; x < map_width; x++) {
            map[map_idx] = map_line_str[x]-48;
            map_idx++;
        }
    }

    return map;
}

void castline(int line) {

    vector cast_dir = ray_dirs[line] * rotate_matrix;
    vector cast_pos = player_pos;

    cast_dir.elems[2] = 0;

    int cell_x = floor(player_pos.elems[0]);
    int cell_y = floor(player_pos.elems[1]);

    float distance = 0;
    float brightness = 1;

    bool x_positive = cast_dir.elems[0] > 0;
    bool y_positive = cast_dir.elems[1] > 0;

    vector col = vector(0);

    int map_pos = 0;
    float x_samp = 0;

    for (int i = 0; i < 100; i++) {
        float x_dist, y_dist;
        x_dist = -fixed_mod(cast_pos.elems[0], 1.0f);
        y_dist = -fixed_mod(cast_pos.elems[1], 1.0f);

        if (!x_positive && x_dist == -0) x_dist = -1;
        if (!y_positive && y_dist == -0) y_dist = -1;

        if (x_positive) x_dist = 1+x_dist;
        if (y_positive) y_dist = 1+y_dist;

        float t_x = x_dist / cast_dir.elems[0];
        float t_y = y_dist / cast_dir.elems[1];

        float t = fmin(t_x, t_y);




        float delta_x = cast_dir.elems[0]*t;
        float delta_y = cast_dir.elems[1]*t;

        distance += fsqrt(pow(delta_x, 2) + pow(delta_y, 2)) * fisheye_factor[line];
        cast_pos.elems[0] += delta_x;
        cast_pos.elems[1] += delta_y;

        if (t_x < t_y) {
            if (x_positive) {cell_x++; brightness = X_POS_LIGHT; x_samp = fixed_mod(cast_pos.elems[1], 1.0f);}
            else            {cell_x--; brightness = X_NEG_LIGHT; x_samp = fixed_mod(cast_pos.elems[1], 1.0f);}
        } else {
            if (y_positive) {cell_y++; brightness = Y_POS_LIGHT; x_samp = fixed_mod(cast_pos.elems[0], 1.0f);}
            else            {cell_y--; brightness = Y_NEG_LIGHT; x_samp = fixed_mod(cast_pos.elems[0], 1.0f);}
        }

        map_pos = map[cell_x + cell_y*map_width];

        if (map_pos)
            break;

        if (cell_x < 0 || cell_x > map_width || cell_y < 0  || cell_y > map_height)
            continue;
    }

    float wall_size = (float)rend.height / distance;
    int wall_size_i = (int)round(wall_size);
    // Clamp wall size
    if (wall_size_i < 0)
        wall_size_i = 0;
    if (wall_size_i >= rend.height)
        wall_size_i = rend.height - 1;

    int empt_size_i = (rend.height - wall_size_i) / 2;

    int overscan = (int)round(wall_size) - wall_size_i;

    int ctr = 0;
    float y_samp = 0;

    for (int i = 0; i < empt_size_i; i++) {
        rend.setpix(line, ctr, 0x00000000);
        ctr++;
    }
    for (int i = 0; i < wall_size_i; i++) {
        y_samp = (float)i / wall_size;
        rend.setpix(line, ctr, (tex0.sample_tex(x_samp, y_samp) * brightness).col_tobytes());
        ctr++;
    }
    for (int i = 0; ctr < rend.height; i++) {
        rend.setpix(line, ctr, 0x00000000);
        ctr++;
    }

    //for (int i = 0; i < rend.height; i++) {
    //    int m_i = i - rend.height/2;
    //    if (m_i < -max || m_i > max)
    //        rend.setpix(line, i, 0x00000000);
    //    else
    //        rend.setpix(line, i, (tex0.sample_tex(x_samp, y_samp)));
    //}
}

void set_rotation(double angle) {
    double s_a = sin(angle);
    double c_a = cos(angle);
    rotate_matrix.elems[0] = c_a;
    rotate_matrix.elems[1] = s_a;
    rotate_matrix.elems[3] = -s_a;
    rotate_matrix.elems[4] = c_a;

    player_dir = vector(-s_a, c_a, 1);
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

    //for (int i = 0; i < 10; i++) {
    //    printf("0x%08x\n", tex0.tex_data[i]);
    //    u_char r, g, b;
    //    asciirenderer::bytetocol(tex0.tex_data[i], r, g, b);
    //    printf("0x%02x, 0x%02x, 0x%02x\n", r, g, b);
    //}
    //exit (0);

    loadmap("maps/map01.txt");

    initscr();

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    rend = asciirenderer();
    rend.dither_enable = true;

    threads = (std::thread*)malloc(rend.width * sizeof(std::thread));
    ray_dirs = (vector*)malloc(rend.width * sizeof(vector));
    fisheye_factor = (float*) malloc(rend.width * sizeof(float));

    std::vector<float> rotate_vec {1, 1, 0, 1, 1, 0, 0, 0, 1};
    std::vector<float> transl_vec {1, 0, 0, 0, 1, 0, 0, 0, 1};

    rotate_matrix.elems = rotate_vec;
    transl_matrix.elems = transl_vec;

    // Precompute ray directions (use the transform matrix to change view dir)
    for (int i = 0; i < rend.width; i++) {
        double dir_angle = (FIELD_OF_VIEW * (M_PI / 180.0)) * ((i - (rend.width / 2.0)) / rend.width);
        ray_dirs[i] = vector(sin(dir_angle), cos(dir_angle), 0);
        fisheye_factor[i] = vector::dot(ray_dirs[i], player_dir);
        ray_dirs[i].elems[2] = 1;
    }

    set_translation(player_pos);

    game_loop();

    free(ray_dirs);
    free(threads);

    exit(0);
}
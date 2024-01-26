//
// Created by loochis on 10/11/23.
//

// REFERENCE
// \033[38;2;<r>;<g>;<b>m # Select RGB foreground color
// \033[48;2;<r>;<g>;<b>m # Select RGB background color

// coord wrapping:
// 0 1 2 3 4 ...
// 5 6 7 8 9 ...
// A B C D E ...

#include <stdio.h>
#include "asciichis.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdexcept>
#include <cmath>
#include <string> // for string class
#include <iostream>
#include <chrono>
#include <thread>

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds
using namespace std;

namespace ASCIICHIS {
    asciirenderer::asciirenderer() {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        width = w.ws_col;
        height = w.ws_row;
        width_c = width/2;
        height_c = height/2;

        vram = (uint32_t*)calloc(width*height, sizeof(uint32_t));

        // Disable the cursor
        printf("\033[?25l");
    }

    asciirenderer::~asciirenderer() = default;

    int asciirenderer::bytetocol(uint32_t col, unsigned char &r, unsigned char &g, unsigned char &b) {
        r = (unsigned char)(col & 0xFF);
        g = (unsigned char)((col >> 8) & 0xFF);
        b = (unsigned char)((col >> 16) & 0xFF);
        return 0;
    }

    volatile int asciirenderer::setpix(int x, int y, uint32_t col) {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return 1;
        vram[idxfromcoord(x, y)] = col;
        return 0;
    }

    int asciirenderer::getpix(int x, int y) {
        return vram[idxfromcoord(x, y)];
    }

    int asciirenderer::drawbox(int x1, int y1, int x2, int y2, uint32_t col) {
        for (int x = x1; x < x2; x++) {
            //float xPerc = (float)(x-x1) / (float)(x2-x1);
            for (int y = y1; y < y2; y++) {
                //float yPerc = (float)(y-y1) / (float)(y2-y1);
                setpix(x, y, col);
            }
        }
        return 0;
    }

    volatile int asciirenderer::idxfromcoord(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= height)
            throw std::out_of_range("Coord->Index is out of bounds");
        return x + y * width;
    }

    int asciirenderer::coordfromidx(int i, int &x, int &y) {
        if (i < 0 || i >= width*height)
            throw std::out_of_range("Index->Coord is out of bounds");
        x = i % width;
        y = i / width;

        return 0;
    }

    // Push to screen
    // \033[38;2;<r>;<g>;<b>m # Select RGB foreground color

    int asciirenderer::pushscreen() {
        int ctr = 0;
        unsigned char r, g, b;
        //string text = "";
        int vram_idx = -1;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                vram_idx++;
                if (dither_enable && dither_state == (x+y) % 2) continue;
                bytetocol(vram[vram_idx], r, g, b);
                printf("\033[%d;%dH", y, x);
                printf("\033[38;2;%d;%d;%dm@", r, g, b);

            }
        }

        dither_state++;
        dither_state %= 2;

        //for (int i = 0; i < width*height; i++) {
        //    bytetocol(vram[i], r, g, b);

            //printf("\033[38;2;%d;%d;%dm@", r, g, b);
            //text += "\033[48;2;0;0;0;38;2;"+to_string(r)+";"+to_string(g)+";"+to_string(b)+"m@";
        //}

        //cout << "\033[2J\033[1;1H";
        cout << "\033[1;1H";
        //sleep_for(milliseconds(2));
        //cout << text.c_str();
        return 0;
    }

    int asciirenderer::flushscreen() {
        for (int i = 0; i < width*height; i++)
            vram[i] = 0;
        return 0;
    }
} // ASCIICHIS
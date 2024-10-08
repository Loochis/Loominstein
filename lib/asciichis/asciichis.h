//
// Created by loochis on 10/11/23.
//

#ifndef ASCII_RENDER_ASCIICHIS_H
#define ASCII_RENDER_ASCIICHIS_H

#include <cstdint>

namespace ASCIICHIS {

    class asciirenderer {
    public:
        bool dither_enable = false;
        int width;
        int height;
        int width_c;
        int height_c;

        int wrap_mode = 0; // 0 = Do Nothing, 1 = Wrap, 2 = Error

        asciirenderer();
        ~asciirenderer();

        static int bytetocol(uint32_t col, unsigned char& r, unsigned char& g, unsigned char& b);

        volatile int setpix(int x, int y, uint32_t col);
        int getpix(int x, int y);
        int drawbox(int x1, int y1, int x2, int y2, uint32_t col);

        int pushscreen();
        int flushscreen();

        volatile uint32_t* vram;
    private:
        int dither_state = 0;
        volatile int idxfromcoord(int x, int y);
        int coordfromidx(int i, int& x, int& y);
    };

} // ASCIICHIS

#endif //ASCII_RENDER_ASCIICHIS_H

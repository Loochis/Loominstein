//
// Created by loochis on 23/01/24.
//

#include "texture.h"
#include <fstream>

using namespace MATHCHIS;

texture::texture(const std::string& path) {
    std::ifstream input( path, std::ios::binary );
    input.read((char*) &width, sizeof(char));
    input.read((char*) &height, sizeof(char));
    input.read((char*) &frames, sizeof(char));

    char tmp; // BYTE ALIGN
    input.read(&tmp, 1);

    tex_data = (uint32_t*) malloc(width*height*sizeof(uint32_t));

    for (int i = 0; i < width*height; i++) {
        input.read((char*) &tex_data[i], 4);
    }
    input.close();
}

vector texture::sample_tex(float x, float y) {
    int x_pos = x*width;
    int y_pos = y*height;
    if (x_pos < 0) x_pos = 0;
    if (y_pos < 0) y_pos = 0;
    if (x_pos >= width) x_pos = width - 1;
    if (y_pos >= height) y_pos = height - 1;

    uint32_t tex_pt = tex_data[x_pos + y_pos*width];

    return vector((float)(tex_pt & 0xFF) / 255.0f, (float)((tex_pt >> 8) & 0xFF) / 255.0f, (float)((tex_pt >> 16) & 0xFF) / 255.0f);

}

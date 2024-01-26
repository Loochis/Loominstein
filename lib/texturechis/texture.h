//
// Created by loochis on 23/01/24.
//

#include <string>
#include "../mathchis/mathchis.h"

#ifndef LOOMINSTEIN_TEXTURE_H
#define LOOMINSTEIN_TEXTURE_H

using namespace MATHCHIS;

class texture {
public:
    uint8_t width;
    uint8_t height;
    uint8_t frames;
    uint32_t* tex_data;

    explicit texture(const std::string& path);

    vector sample_tex(float x, float y);
};


#endif //LOOMINSTEIN_TEXTURE_H

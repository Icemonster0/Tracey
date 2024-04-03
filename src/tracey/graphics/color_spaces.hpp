#ifndef COLOR_SPACES_HPP
#define COLOR_SPACES_HPP

#include "../../lib/glm.hpp"
#include "buffer.hpp"

namespace trc::color {

enum ColorMode {STANDARD, FILMIC, RAW};

extern ColorMode color_mode;

glm::vec3 raw_to_sRGB(glm::vec3 color);
float raw_to_sRGB(float color);
glm::vec3 sRGB_to_raw(glm::vec3 color);
float sRGB_to_raw(float color);

// I tried implementing ACES, got confused and gave up...
// Now I'm only using the UE5 S-curve for a filmic look:
// https://docs.unrealengine.com/5.3/en-US/color-grading-and-the-filmic-tonemapper-in-unreal-engine/
glm::vec3 ACEScg_to_raw(glm::vec3 color);
float ACEScg_to_raw(float color);
glm::vec3 raw_to_ACEScg(glm::vec3 color);
float raw_to_ACEScg(float color);

glm::vec3 raw_to_filmic(glm::vec3 color);
float raw_to_filmic(float color);

template<typename T>
T input_transform(T color) {
    return sRGB_to_raw(color);
    // switch (color_mode) {
    //     case STANDARD: return sRGB_to_raw(color);
    //     case FILMIC: return sRGB_to_raw(color);
    //     // case FILMIC: return raw_to_ACEScg(sRGB_to_raw(color));
    //     case RAW: return sRGB_to_raw(color);
    //     default: return sRGB_to_raw(color);
    // }
}

template<typename T>
T output_transform(T color, float exposure) {
    color *= powf(2.f, exposure);
    switch (color_mode) {
        case STANDARD: return raw_to_sRGB(color);
        case FILMIC: return raw_to_sRGB(raw_to_filmic(color));
        // case FILMIC: return raw_to_filmic(raw_to_sRGB(ACEScg_to_raw(color)));
        case RAW: return color;
        default: return color;
    }
}

template<typename T>
void apply_output_transform_to_buffer(Buffer<T> &buf, float exposure) {
    glm::ivec2 size = buf.get_size();
    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            *buf.at({x, y}) = output_transform(*buf.at({x, y}), exposure);
        }
    }
}

} /* trc::color */

#endif /* end of include guard: COLOR_SPACES_HPP */

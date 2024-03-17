#ifndef COLOR_SPACES_HPP
#define COLOR_SPACES_HPP

#include "../../lib/glm.hpp"

namespace trc::color {

enum ColorMode {STANDARD, FILMIC, RAW};

extern ColorMode color_mode;

glm::vec3 raw_to_sRGB(glm::vec3 color);
float raw_to_sRGB(float color);
glm::vec3 sRGB_to_raw(glm::vec3 color);
float sRGB_to_raw(float color);

glm::vec3 ACEScg_to_raw(glm::vec3 color);
float ACEScg_to_raw(float color);
glm::vec3 raw_to_ACEScg(glm::vec3 color);
float raw_to_ACEScg(float color);

glm::vec3 ACEScg_to_filmic(glm::vec3 color);
float ACEScg_to_filmic(float color);

template<typename T>
T input_transform(T color) {
    switch (color_mode) {
        case STANDARD: return sRGB_to_raw(color);
        case FILMIC: return raw_to_ACEScg(sRGB_to_raw(color));
        case RAW: return sRGB_to_raw(color);
        default: return sRGB_to_raw(color);
    }
}

template<typename T>
T output_transform(T color, float exposure) {
    color *= powf(2.f, exposure);
    switch (color_mode) {
        case STANDARD: return raw_to_sRGB(color);
        case FILMIC: return raw_to_sRGB(ACEScg_to_filmic(color));
        case RAW: return color;
        default: return color;
    }
}

} /* trc::color */

#endif /* end of include guard: COLOR_SPACES_HPP */

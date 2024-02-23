#include "color_spaces.hpp"

namespace trc::color {

glm::vec3 to_sRGB(glm::vec3 color) {
    const float c = 1.f / 2.2f;
    return glm::vec3 {
        powf(color.x, c),
        powf(color.y, c),
        powf(color.z, c)
    };
}

} /* trc::color */

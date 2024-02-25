#ifndef COLOR_SPACES_HPP
#define COLOR_SPACES_HPP

#include "../../lib/glm.hpp"

namespace trc::color {

glm::vec3 to_sRGB(glm::vec3 color);

glm::vec3 sRGB_to_flat(glm::vec3 color);

} /* trc::color */

#endif /* end of include guard: COLOR_SPACES_HPP */

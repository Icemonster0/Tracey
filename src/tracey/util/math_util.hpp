#ifndef MATH_UTIL_HPP
#define MATH_UTIL_HPP

#include "../../lib/glm.hpp"
#include "random.hpp"

namespace trc::math {

float fast_inv_sqrt(float number);

glm::vec3 random_dir_in_hemisphere(glm::vec3 normal, RNG *rng);

} /* trc::math */

#endif /* end of include guard: MATH_UTIL_HPP */

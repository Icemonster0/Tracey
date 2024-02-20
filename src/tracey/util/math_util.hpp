#ifndef MATH_UTIL_HPP
#define MATH_UTIL_HPP

#include <cmath>
#include <optional>

#include "../../lib/glm.hpp"
#include "random.hpp"
#include "normal_dist.hpp"
#include "../geometry/ray.hpp"

namespace trc::math {

float fast_inv_sqrt(float number);
glm::vec3 normalize(glm::vec3 vec);

glm::vec3 random_dir_in_hemisphere(glm::vec3 normal, RNG *rng);

float fresnel(float n1, float n2, glm::vec3 incident, glm::vec3 normal);

std::optional<float> intersect_sphere(Ray ray, glm::vec3 center, float radius);

} /* trc::math */

#endif /* end of include guard: MATH_UTIL_HPP */

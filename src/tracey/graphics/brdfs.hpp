#ifndef BRDFS_HPP
#define BRDFS_HPP

#include "../../lib/glm.hpp"
#include "../util/random.hpp"

namespace trc::brdf {

float flat(glm::vec3 in, glm::vec3 out, glm::vec3 normal, float rough);

float ggx(glm::vec3 in, glm::vec3 out, glm::vec3 normal, float rough);
glm::vec3 ggx_normal(glm::vec3 g_normal, float rough, RNG *rng);

glm::vec3 randomize_ray(glm::vec3 ray, float theta, RNG *rng);

} /* trc::brdf */

#endif /* end of include guard: BRDFS_HPP */

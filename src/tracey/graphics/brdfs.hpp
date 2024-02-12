#ifndef BRDFS_HPP
#define BRDFS_HPP

#include "../../lib/glm.hpp"
#include "../util/random.hpp"

namespace trc::brdf {

float flat(glm::vec3 in, glm::vec3 out, glm::vec3 normal, float rough);
float ggx(glm::vec3 in, glm::vec3 out, glm::vec3 normal, float rough);
glm::vec3 ggx_inverse(glm::vec3 out, glm::vec3 g_normal, float rough, RNG *rng);

} /* trc::brdf */

#endif /* end of include guard: BRDFS_HPP */

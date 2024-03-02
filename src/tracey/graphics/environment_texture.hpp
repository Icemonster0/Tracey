#ifndef ENVIRONMENT_TEXTURE_HPP
#define ENVIRONMENT_TEXTURE_HPP

#include <memory>

#include "../../lib/glm.hpp"
#include "attribute.hpp"

namespace trc {

class EnvironmentTexture {
public:
    EnvironmentTexture(const char *file_path, float p_rotation = 0.f);
    EnvironmentTexture();

    int load(const char *file_path);
    void set_color(glm::vec3 color);

    glm::vec3 sample(glm::vec3 ray_dir);
    glm::vec3 sample(glm::vec2 tex_coord);

    void set_rotation(float p_rotation);
    float get_rotation();

private:
    std::unique_ptr<Attrib<glm::vec3>> texture;
    float rotation;
};

} /* trc */

#endif /* end of include guard: ENVIRONMENT_TEXTURE_HPP */

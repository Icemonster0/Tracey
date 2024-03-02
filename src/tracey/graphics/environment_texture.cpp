#include "environment_texture.hpp"

#include "image_read_write.hpp"
#include "../util/math_util.hpp"

namespace trc {

EnvironmentTexture::EnvironmentTexture(const char *file_path, float p_rotation)
    : rotation(p_rotation) {

    load(file_path);
}

EnvironmentTexture::EnvironmentTexture() {}

int EnvironmentTexture::load(const char *file_path) {
    AttribTexture<glm::vec3> *attrib = new AttribTexture<glm::vec3> {};
    int result = image_rw::read_hdr_texture(file_path, *attrib);

    if (result) {
        // create magenta 'missing' texture
        texture = std::unique_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {glm::vec3 {1.f, 0.f, 1.f}}};
        delete attrib;
        printf("Missing environment texture: '%s'\n", file_path);
    } else {
        // use loaded texture
        texture = std::unique_ptr<Attrib<glm::vec3>> {attrib};
        printf("Loading environment texture: '%s'\n", file_path);
    }

    return result;
}

void EnvironmentTexture::set_color(glm::vec3 color) {
    texture = std::unique_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {color}};
}

glm::vec3 EnvironmentTexture::sample(glm::vec3 ray_dir) {
    return sample(math::sphere_coords(glm::rotateY(ray_dir, glm::radians(rotation))));
}

glm::vec3 EnvironmentTexture::sample(glm::vec2 tex_coord) {
    return texture->sample(tex_coord);
}

void EnvironmentTexture::set_rotation(float p_rotation) {
    rotation = p_rotation;
}

float EnvironmentTexture::get_rotation() {
    return rotation;
}

} /* trc */

#ifndef ATTRIBUTES_HPP
#define ATTRIBUTES_HPP

#include "../../lib/glm.hpp"
#include "buffer.hpp"

namespace trc {

template <typename T>
struct Attrib {
    virtual T sample(glm::vec2 tex_coord) = 0;
};

template <typename T>
struct AttribValue : public Attrib<T> {
    AttribValue(T init) : value(init) {}
    AttribValue() : value(T {}) {}

    T sample(glm::vec2 tex_coord) {
        return value;
    }

private:
    T value;
};

template <typename T>
struct AttribTexture : public Attrib<T> {
    AttribTexture(glm::vec2 size, T init) : texture(Buffer<T> {size, init}) {}
    AttribTexture(glm::vec2 size) : texture(Buffer<T> {size}) {}
    AttribTexture() : texture(Buffer<T> {}) {}

    T sample(glm::vec2 tex_coord) {
        glm::vec2 image_coord = tex_coord * glm::vec2 {get_size()};
        glm::vec2 fract_coord = glm::fract(image_coord);

        glm::ivec2 a_pos = glm::clamp(glm::ivec2 {glm::floor(image_coord.x), glm::floor(image_coord.y)}, glm::ivec2 {0}, get_size());
        glm::ivec2 b_pos = glm::clamp(glm::ivec2 {glm::ceil(image_coord.x), glm::floor(image_coord.y)}, glm::ivec2 {0}, get_size());
        glm::ivec2 c_pos = glm::clamp(glm::ivec2 {glm::floor(image_coord.x), glm::ceil(image_coord.y)}, glm::ivec2 {0}, get_size());
        glm::ivec2 d_pos = glm::clamp(glm::ivec2 {glm::ceil(image_coord.x), glm::ceil(image_coord.y)}, glm::ivec2 {0}, get_size());

        T ab = glm::mix(*texture.at(a_pos), *texture.at(b_pos), fract_coord.x);
        T cd = glm::mix(*texture.at(c_pos), *texture.at(c_pos), fract_coord.x);
        return glm::mix(ab, cd, fract_coord.y);
    }

    glm::ivec2 get_size() { return texture.get_size(); }
    void set_size(glm::ivec2 size) { texture.fill(size, T {}); }
    void fill(T value) { texture.fill(value); }
    void set_pixel(glm::ivec2 coord, T value) { *texture.at(coord) = value; }

private:
    Buffer<T> texture;
};

} /* trc */

#endif /* end of include guard: ATTRIBUTES_HPP */

#include "material.hpp"

namespace trc {

Material::Material(glm::vec3 a, float r, float m, glm::vec3 e, glm::vec3 n) {
    albedo = std::unique_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {a}};
    roughness = std::unique_ptr<Attrib<float>> {new AttribValue<float> {r}};
    metallic = std::unique_ptr<Attrib<float>> {new AttribValue<float> {m}};
    emission = std::unique_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {e}};
    normal = std::unique_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {n}};
}

Material::Material() {}

} /* trc */

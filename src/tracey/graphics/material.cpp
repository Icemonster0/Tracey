#include "material.hpp"

namespace trc {

Material::Material(glm::vec3 a, float r, float m, glm::vec3 e, glm::vec3 n, float t, float i) {
    albedo = std::shared_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {a}};
    roughness = std::shared_ptr<Attrib<float>> {new AttribValue<float> {r}};
    metallic = std::shared_ptr<Attrib<float>> {new AttribValue<float> {m}};
    emission = std::shared_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {e}};
    normal = std::shared_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {n}};
    transmissive = std::shared_ptr<Attrib<float>> {new AttribValue<float> {t}};
    ior = std::shared_ptr<Attrib<float>> {new AttribValue<float> {i}};
    clamp_attribs();
}

Material::Material(std::shared_ptr<Attrib<glm::vec3>> a,
                   std::shared_ptr<Attrib<float>> r,
                   std::shared_ptr<Attrib<float>> m,
                   std::shared_ptr<Attrib<glm::vec3>> e,
                   std::shared_ptr<Attrib<glm::vec3>> n,
                   std::shared_ptr<Attrib<float>> t,
                   std::shared_ptr<Attrib<float>> i)
    : albedo(a), roughness(r), metallic(m), emission(e), normal(n), transmissive(t), ior(i) {
    clamp_attribs();
}

Material::Material() {}

void Material::clamp_attribs() {
    albedo->clamp(glm::vec3 {0.001f}, glm::vec3 {0.999f});
    roughness->clamp(0.0001f, 0.9999f);
    metallic->clamp(0.f, 1.f);
}

} /* trc */

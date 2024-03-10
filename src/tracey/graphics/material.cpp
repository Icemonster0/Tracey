#include "material.hpp"

namespace trc {

Material::Material(glm::vec3 a, float r, float m, glm::vec3 e, glm::vec3 n, float t, float i, float o) {
    albedo = std::shared_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {a}};
    roughness = std::shared_ptr<Attrib<float>> {new AttribValue<float> {r}};
    metallic = std::shared_ptr<Attrib<float>> {new AttribValue<float> {m}};
    emission = std::shared_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {e}};
    normal = std::shared_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {n}};
    transmissive = std::shared_ptr<Attrib<float>> {new AttribValue<float> {t}};
    ior = std::shared_ptr<Attrib<float>> {new AttribValue<float> {i}};
    alpha = std::shared_ptr<Attrib<float>> {new AttribValue<float> {o}};
    clamp_attribs();
}

Material::Material(std::shared_ptr<Attrib<glm::vec3>> a,
                   std::shared_ptr<Attrib<float>> r,
                   std::shared_ptr<Attrib<float>> m,
                   std::shared_ptr<Attrib<glm::vec3>> e,
                   std::shared_ptr<Attrib<glm::vec3>> n,
                   std::shared_ptr<Attrib<float>> t,
                   std::shared_ptr<Attrib<float>> i,
                   std::shared_ptr<Attrib<float>> o)
    : albedo(a), roughness(r), metallic(m), emission(e), normal(n), transmissive(t), ior(i), alpha(o) {
    clamp_attribs();
}

Material::Material() {
    albedo = std::shared_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {glm::vec3 {1.f}}};
    roughness = std::shared_ptr<Attrib<float>> {new AttribValue<float> {0.5f}};
    metallic = std::shared_ptr<Attrib<float>> {new AttribValue<float> {0.f}};
    emission = std::shared_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {glm::vec3 {0.f}}};
    normal = std::shared_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {glm::vec3 {0.5f, 0.5f, 1.f}}};
    transmissive = std::shared_ptr<Attrib<float>> {new AttribValue<float> {0.f}};
    ior = std::shared_ptr<Attrib<float>> {new AttribValue<float> {1.6f}};
    alpha = std::shared_ptr<Attrib<float>> {new AttribValue<float> {1.f}};
}

void Material::clamp_attribs() {
    albedo->clamp(glm::vec3 {0.001f}, glm::vec3 {0.999f});
    roughness->clamp(0.0001f, 0.9999f);
    metallic->clamp(0.f, 1.f);
    alpha->clamp(0.f, 1.f);
}

} /* trc */

#include "material.hpp"

namespace trc {

Material::Material(glm::vec3 a, float r, float m, glm::vec3 e, glm::vec3 n) {
    // albedo = std::unique_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {a}};
    // roughness = std::unique_ptr<Attrib<float>> {new AttribValue<float> {r}};
    // metallic = std::unique_ptr<Attrib<float>> {new AttribValue<float> {m}};
    // emission = std::unique_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {e}};
    // normal = std::unique_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {n}};
    albedo = std::shared_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {a}};
    roughness = std::shared_ptr<Attrib<float>> {new AttribValue<float> {r}};
    metallic = std::shared_ptr<Attrib<float>> {new AttribValue<float> {m}};
    emission = std::shared_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {e}};
    normal = std::shared_ptr<Attrib<glm::vec3>> {new AttribValue<glm::vec3> {n}};
}

// Material::Material(std::unique_ptr<Attrib<glm::vec3>> a,
    //                std::unique_ptr<Attrib<float>> r,
    //                std::unique_ptr<Attrib<float>> m,
    //                std::unique_ptr<Attrib<glm::vec3>> e,
    //                std::unique_ptr<Attrib<glm::vec3>> n)
Material::Material(std::shared_ptr<Attrib<glm::vec3>> a,
                   std::shared_ptr<Attrib<float>> r,
                   std::shared_ptr<Attrib<float>> m,
                   std::shared_ptr<Attrib<glm::vec3>> e,
                   std::shared_ptr<Attrib<glm::vec3>> n)
    : albedo(a), roughness(r), metallic(m), emission(e), normal(n) {}

Material::Material() {}

} /* trc */

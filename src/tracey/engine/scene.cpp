#include "scene.hpp"

namespace trc {

Scene::Scene() {
    env_texture.set_color(glm::vec3 {0.05f});
}

Shape *Scene::add_object(std::unique_ptr<Shape> obj) {
    Shape *ptr = obj.get();
    object_list.push_back(std::move(obj));
    return ptr;
}

Light *Scene::add_light(std::unique_ptr<Light> light) {
    Light *ptr = light.get();
    light_list.push_back(std::move(light));
    return ptr;
}

Material *Scene::add_material(std::unique_ptr<Material> material) {
    Material *ptr = material.get();
    material_list.push_back(std::move(material));
    return ptr;
}

Mesh *Scene::add_mesh(std::unique_ptr<Mesh> mesh) {
    Mesh *ptr = mesh.get();
    mesh_list.push_back(std::move(mesh));
    return ptr;
}

std::list<std::unique_ptr<Shape>> *Scene::get_object_list() {
    return &object_list;
}

std::list<std::unique_ptr<Light>> *Scene::get_light_list() {
    return &light_list;
}

std::list<std::unique_ptr<Material>> *Scene::get_material_list() {
    return &material_list;
}

std::list<std::unique_ptr<Mesh>> *Scene::get_mesh_list() {
    return &mesh_list;
}

EnvironmentTexture *Scene::set_environment(EnvironmentTexture tex) {
    env_texture = std::move(tex);
    return &env_texture;
}

EnvironmentTexture *Scene::get_environment() {
    return &env_texture;
}

void Scene::append(Scene &other_scene) {
    object_list.splice(object_list.end(), other_scene.object_list);
    light_list.splice(light_list.end(), other_scene.light_list);
    material_list.splice(material_list.end(), other_scene.material_list);
    mesh_list.splice(mesh_list.end(), other_scene.mesh_list);
}

} /* trc */

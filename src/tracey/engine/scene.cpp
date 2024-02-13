#include "scene.hpp"

namespace trc {

Scene::Scene() {}

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

std::list<std::unique_ptr<Shape>> *Scene::get_object_list() {
    return &object_list;
}

std::list<std::unique_ptr<Light>> *Scene::get_light_list() {
    return &light_list;
}

std::list<std::unique_ptr<Material>> *Scene::get_material_list() {
    return &material_list;
}

} /* trc */

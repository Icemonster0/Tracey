#include "scene.hpp"

namespace trc {

Scene::Scene() {}

void Scene::add_object(std::unique_ptr<Shape> obj) {
    object_list.push_back(std::move(obj));
}

void Scene::add_light(std::unique_ptr<Light> light) {
    light_list.push_back(std::move(light));
}

std::list<std::unique_ptr<Shape>> *Scene::get_object_list() {
    return &object_list;
}

std::list<std::unique_ptr<Light>> *Scene::get_light_list() {
    return &light_list;
}

} /* trc */

#include "scene.hpp"

namespace trc {

Scene::Scene() {}

void Scene::add_object(std::unique_ptr<Shape> obj) {
    object_list.push_back(std::move(obj));
}

std::list<std::unique_ptr<Shape>> *Scene::get_object_list() {
    return &object_list;
}

} /* trc */

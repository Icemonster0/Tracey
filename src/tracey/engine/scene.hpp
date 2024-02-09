#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include <memory>

#include "../../glm.hpp"
#include "../geometry/shapes/shapes.hpp"
#include "../geometry/lights/lights.hpp"

namespace trc {

class Scene {
public:
    Scene();

    void add_object(std::unique_ptr<Shape> obj);
    void add_light(std::unique_ptr<Light> light);

    std::list<std::unique_ptr<Shape>> *get_object_list();
    std::list<std::unique_ptr<Light>> *get_light_list();

private:
    std::list<std::unique_ptr<Shape>> object_list;
    std::list<std::unique_ptr<Light>> light_list;
};

} /* trc */

#endif /* end of include guard: SCENE_HPP */

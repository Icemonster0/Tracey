#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include <memory>

#include "../../glm.hpp"
#include "../geometry/shape.hpp"

namespace trc {

class Scene {
public:
    Scene();

    void add_object(std::unique_ptr<Shape> obj);
    std::list<std::unique_ptr<Shape>> *get_object_list();

private:
    std::list<std::unique_ptr<Shape>> object_list;
};

} /* trc */

#endif /* end of include guard: SCENE_HPP */

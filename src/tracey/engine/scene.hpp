#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include <memory>

#include "../../lib/glm.hpp"
#include "../geometry/shapes/shapes.hpp"
#include "../geometry/mesh.hpp"
#include "../geometry/lights/lights.hpp"

namespace trc {

class Scene {
public:
    Scene();

    Shape *add_object(std::unique_ptr<Shape> obj);
    Light *add_light(std::unique_ptr<Light> light);
    Material *add_material(std::unique_ptr<Material> material);
    Mesh *add_mesh(std::unique_ptr<Mesh> mesh);

    std::list<std::unique_ptr<Shape>> *get_object_list();
    std::list<std::unique_ptr<Light>> *get_light_list();
    std::list<std::unique_ptr<Material>> *get_material_list();
    std::list<std::unique_ptr<Mesh>> *get_mesh_list();

private:
    std::list<std::unique_ptr<Shape>> object_list;
    std::list<std::unique_ptr<Light>> light_list;
    std::list<std::unique_ptr<Material>> material_list;
    std::list<std::unique_ptr<Mesh>> mesh_list;
};

} /* trc */

#endif /* end of include guard: SCENE_HPP */

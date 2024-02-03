#ifndef VIEWER_HPP
#define VIEWER_HPP

#include "../../glm.hpp"
#include "input_package.hpp"
#include "../geometry/camera.hpp"

namespace trc {

class Viewer {
public:
    Viewer(glm::ivec2 window_size);
    Viewer();

    void update(InputPackage input, float delta_t);

    Camera *get_camera();

private:
    Camera camera;

    float look_sens;
    float move_speed;
};

} /* trc */

#endif /* end of include guard: VIEWER_HPP */

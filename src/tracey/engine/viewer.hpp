#ifndef VIEWER_HPP
#define VIEWER_HPP

#include "../../lib/glm.hpp"
#include "input_package.hpp"
#include "../geometry/camera.hpp"

namespace trc {

class Viewer {
public:
    Viewer(glm::ivec2 window_size, float fov, float focal_length = 10.f, float aperture = 0.f);
    Viewer();

    void update(InputPackage input, float delta_t, glm::ivec2 window_size);

    Camera *get_camera();
    float get_speed();

private:
    Camera camera;
    glm::ivec2 window_size_cache;

    float look_sens;
    float scroll_fac;
    float move_speed;
};

} /* trc */

#endif /* end of include guard: VIEWER_HPP */

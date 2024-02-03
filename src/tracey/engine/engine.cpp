#include "engine.hpp"

#include <cstdio>
#include <cstdlib>
#include <sstream>

#include "../../glm.hpp"
#include "../../glfw.hpp"
#include "input_package.hpp"

namespace trc {

Engine::Engine() {}

int Engine::run() {
    scene = Scene {};
    window_manager = WindowManager {glm::ivec2 {400, 300}};
    viewer = Viewer {window_manager.get_size()};
    sampler = Sampler {window_manager.get_size()};
    console = Console {30.0f};

    float last_frame_t = glfwGetTime();
    float this_frame_t = glfwGetTime();
    float delta_t = 0.f;

    while (!window_manager.window_should_close()) {
        sampler.render(window_manager.get_size());
        window_manager.draw_frame(sampler.get_frame_buffer());
        InputPackage input = window_manager.handle_events();
        viewer.update(input, delta_t);

        console.print(
            delta_t,
            window_manager.get_size(),
            viewer.get_camera()->get_pos(),
            viewer.get_camera()->get_yaw(),
            viewer.get_camera()->get_pitch(),
            viewer.get_camera()->get_fov()
        );

        this_frame_t = glfwGetTime();
        delta_t = this_frame_t - last_frame_t;
        last_frame_t = this_frame_t;
    }

    window_manager.close();
    console.close();
    return 0;
}

} /* trc */

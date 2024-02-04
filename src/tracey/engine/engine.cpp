#include "engine.hpp"

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <memory>

#include "../../glm.hpp"
#include "../../glfw.hpp"
#include "input_package.hpp"
#include "../geometry/shape.hpp"
#include "../geometry/shapes/shapes.hpp"

namespace trc {

Engine::Engine() {}

int Engine::run() {
    scene = Scene {};
    scene.add_object(std::unique_ptr<Shape>(new GroundPlane(0.f, Shader {})));
    scene.add_object(std::unique_ptr<Shape>(new Sphere(glm::vec3 {0.f, 1.0f, 0.f}, 1.f, Shader {})));
    scene.add_object(std::unique_ptr<Shape>(new Sphere(glm::vec3 {3.f, 2.0f, 2.f}, 1.f, Shader {})));
    scene.add_object(std::unique_ptr<Shape>(new Sphere(glm::vec3 {2.f, 0.0f, -4.f}, 1.f, Shader {})));

    accelerator = Accelerator {&scene};
    window_manager = WindowManager {glm::ivec2 {400, 300}};
    viewer = Viewer {window_manager.get_size()};
    sampler = Sampler {window_manager.get_size()};
    console = Console {5.0f};

    float last_frame_t = glfwGetTime();
    float this_frame_t = glfwGetTime();
    float delta_t = 0.f;

    while (!window_manager.window_should_close()) {
        sampler.render(window_manager.get_size(), viewer.get_camera(), &accelerator);
        window_manager.draw_frame(sampler.get_frame_buffer());
        InputPackage input = window_manager.handle_events();
        viewer.update(input, delta_t, window_manager.get_size());

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

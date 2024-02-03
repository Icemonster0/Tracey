#include "engine.hpp"

#include <cstdio>
#include <cstdlib>
#include <sstream>

#include "../../glm.hpp"
#include "../../glfw.hpp"

namespace trc {

Engine::Engine() {}

int Engine::run() {
    window_manager = WindowManager {glm::ivec2 {400, 300}};
    sampler = Sampler {window_manager.get_size()};
    console = Console {2.0f};

    float last_frame_t = glfwGetTime();
    float this_frame_t;

    while (!window_manager.window_should_close()) {
        sampler.render(window_manager.get_size());
        window_manager.draw_frame(sampler.get_frame_buffer());
        window_manager.handle_events();

        this_frame_t = glfwGetTime();
        console.print(
            this_frame_t - last_frame_t,
            window_manager.get_size()
        );
        last_frame_t = this_frame_t;
    }

    window_manager.close();
    console.close();
    return 0;
}

} /* trc */

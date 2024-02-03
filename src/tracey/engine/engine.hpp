#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "window_manager.hpp"
#include "sampler.hpp"
#include "console.hpp"
#include "../scene/scene.hpp"
#include "viewer.hpp"

namespace trc {

class Engine {
public:
    Engine();

    int run();

private:
    Scene scene;
    WindowManager window_manager;
    Viewer viewer;
    Sampler sampler;
    Console console;
};

} /* trc */

#endif /* end of include guard: ENGINE_HPP */

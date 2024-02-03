#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "window_manager.hpp"
#include "sampler.hpp"
#include "console.hpp"

namespace trc {

class Engine {
public:
    Engine();

    int run();

private:
    WindowManager window_manager;
    Sampler sampler;
    Console console;
};

} /* trc */

#endif /* end of include guard: ENGINE_HPP */

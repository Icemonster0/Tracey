#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "user_config.hpp"
#include "../graphics/shader_pack.hpp"
#include "scene.hpp"
#include "accelerator.hpp"
#include "window_manager.hpp"
#include "viewer.hpp"
#include "sampler.hpp"
#include "console.hpp"

namespace trc {

class Engine {
public:
    Engine(UserConfig cfg);

    int run();

private:
    UserConfig cfg;
    ShaderPack shader_pack;
    Scene scene;
    Accelerator accelerator;
    WindowManager window_manager;
    Viewer viewer;
    Sampler sampler;
    Console console;
};

} /* trc */

#endif /* end of include guard: ENGINE_HPP */

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <memory>

#include "../../lib/glm.hpp"
#include "../util/random.hpp"

#include "user_config.hpp"
#include "../graphics/shader_pack.hpp"
#include "scene.hpp"
#include "accelerator.hpp"
#include "bvh.hpp"
#include "window_manager.hpp"
#include "viewer.hpp"
#include "sampler.hpp"
#include "console.hpp"

namespace trc {

class Engine {
public:
    Engine(UserConfig cfg);

    int load_file(std::string file_path);
    int run();

private:
    UserConfig cfg;
    ShaderPack shader_pack;
    Scene scene;
    std::unique_ptr<Accelerator> accelerator;
    WindowManager window_manager;
    Viewer viewer;
    Sampler sampler;
    Console console;

    bool preview_mode;
    int error;

    int render_image(glm::ivec2 image_size, int samples, std::mt19937 *seed_gen);

    // Tests
    void test_scene_setup();
    void test_integrals();
};

} /* trc */

#endif /* end of include guard: ENGINE_HPP */

#include "engine.hpp"

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <memory>

#include "../../lib/glfw.hpp"
#include "input_package.hpp"
#include "../geometry/shape.hpp"
#include "../geometry/shapes/shapes.hpp"
#include "../graphics/image_read_write.hpp"

namespace trc {

Engine::Engine(UserConfig cfg) : cfg(cfg), error(0), preview_mode(true) {}

int Engine::run() {
    scene_setup();

    accelerator = Accelerator {&scene};
    window_manager = WindowManager {cfg.window_size};
    viewer = Viewer {window_manager.get_size(), cfg.fov};
    sampler = Sampler {window_manager.get_size()};

    std::random_device rand_dev;
    std::mt19937 seed_gen {rand_dev()};

    float last_frame_t = glfwGetTime();
    float this_frame_t = glfwGetTime();
    float delta_t = 0.f;

    while (!window_manager.window_should_close()) {
        sampler.render_frame(
            window_manager.get_size(),
            viewer.get_camera(),
            &accelerator,
            &shader_pack,
            seed_gen(),
            window_manager.is_update_required(),
            cfg.samples,
            preview_mode
        );
        window_manager.draw_frame(sampler.get_frame_buffer());
        InputPackage input = window_manager.handle_events();
        viewer.update(input, delta_t, window_manager.get_size());

        if (input.r) error = render_image(cfg.render_size, cfg.samples, &seed_gen);
        if (input.p) preview_mode = !preview_mode;

        if (error) break;

        console.print(
            cfg.console_frequency,
            delta_t,
            preview_mode,
            sampler.get_samples(),
            cfg.samples,
            window_manager.get_size(),
            viewer.get_camera()->get_pos(),
            viewer.get_camera()->get_yaw(),
            viewer.get_camera()->get_pitch(),
            viewer.get_camera()->get_fov(),
            viewer.get_speed()
        );

        this_frame_t = glfwGetTime();
        delta_t = this_frame_t - last_frame_t;
        last_frame_t = this_frame_t;
    }

    window_manager.close();
    console.clear();
    return error;
}

void Engine::scene_setup() {
    /* SPHERE GROUP SETUP */
    Material *mat_floor = scene.add_material(std::make_unique<Material>(glm::vec3 {0.9f, 0.9f, 0.9f}, 0.5f, 0.f));
    Material *mat_a = scene.add_material(std::make_unique<Material>(glm::vec3 {1.0f, 0.5f, 0.0f}, 0.001f, 1.f));
    Material *mat_b = scene.add_material(std::make_unique<Material>(glm::vec3 {0.9f, 0.2f, 0.2f}, 0.03f, 0.f));
    Material *mat_c = scene.add_material(std::make_unique<Material>(glm::vec3 {0.2f, 0.9f, 0.3f}, 1.f, 0.f));
    Material *mat_d = scene.add_material(std::make_unique<Material>(glm::vec3 {0.8f, 0.8f, 0.8f}, 0.f, 0.f, glm::vec3 {0.f}, glm::vec3 {0.f}, 1.f, 1.5f));

    scene.add_object(std::unique_ptr<Shape>(new GroundPlane(0.f, shader_pack.shader_combined.get(), mat_floor)));
    scene.add_object(std::unique_ptr<Shape>(new Sphere(glm::vec3 {0.f, 0.f, 1015.f}, 1000.f, shader_pack.shader_combined.get(), mat_floor)));
    scene.add_object(std::unique_ptr<Shape>(new Sphere(glm::vec3 {1.2f, 1.0f, 1.2f}, 1.f, shader_pack.shader_combined.get(), mat_a)));
    scene.add_object(std::unique_ptr<Shape>(new Sphere(glm::vec3 {-1.2f, 1.0f, 0.f}, 1.f, shader_pack.shader_combined.get(), mat_b)));
    scene.add_object(std::unique_ptr<Shape>(new Sphere(glm::vec3 {1.1f, 0.5f, -0.5f}, 0.5f, shader_pack.shader_combined.get(), mat_c)));
    scene.add_object(std::unique_ptr<Shape>(new Sphere(glm::vec3 {0.0f, 0.6f, -1.1f}, 0.6f, shader_pack.shader_combined.get(), mat_d)));

    scene.add_light(std::unique_ptr<Light>(new PointLight(glm::vec3 {2.5f, 4.0f, 2.5f}, glm::vec3 {0.8f, 0.9f, 1.0f}, 40.f, 0.1f)));
    scene.add_light(std::unique_ptr<Light>(new PointLight(glm::vec3 {-2.f, 3.0f, -3.f}, glm::vec3 {0.9f, 0.6f, 1.0f}, 20.f, 1.f)));
    // scene.add_light(std::unique_ptr<Light>(new SunLight(glm::vec3 {0.5f, -1.f, 0.5f}, glm::vec3 {1.f, 0.95f, 0.95f}, 3.0f, 0.526f)));


    /* SPHERE ROW SETUP */
    // scene.add_light(std::unique_ptr<Light>(new SunLight(glm::vec3 {0.f, -0.5f, -0.5f}, glm::vec3 {1.f}, 3.f, 5.f)));
    //
    // Material *mat_floor = scene.add_material(std::make_unique<Material>(glm::vec3 {1.f}, 0.0f, 0.0f));
    // scene.add_object(std::unique_ptr<Shape>(new GroundPlane(0.f, shader_pack.shader_combined.get(), mat_floor)));
    //
    // const int max_i = 10;
    // for (int i = 0; i <= max_i; ++i) {
    //     Material *mat = scene.add_material(std::make_unique<Material>(glm::vec3 {1.f, 0.f, 0.f}, (float)i / (float)max_i, 1.f));
    //     scene.add_object(std::unique_ptr<Shape>(new Sphere(glm::vec3 {i * 2.2f, 1.f, 0.f}, 1.f, shader_pack.shader_combined.get(), mat)));
    // }
}

int Engine::render_image(glm::ivec2 image_size, int samples, std::mt19937 *seed_gen) {
    InputPackage input = window_manager.handle_events();

    int return_code = 0;

    sampler.initialize_image(image_size);

    Camera render_camera = *viewer.get_camera();
    render_camera.set_aspect((float)image_size.x / (float)image_size.y);

    float start_t = glfwGetTime();
    float last_t = glfwGetTime();
    float this_t = glfwGetTime();
    float delta_t = 0.f;
    float render_time = 0.f;
    float sample_rate = 1.f;

    int sample = 0;
    while (!input.i && !window_manager.window_should_close() && sample < samples) {

        ++sample;
        sampler.render_image_sample(
            &render_camera,
            &accelerator,
            &shader_pack,
            seed_gen->operator()(),
            sample
        );

        input = window_manager.handle_events();

        console.print_render_info(
            cfg.console_frequency,
            delta_t,
            sample,
            cfg.samples,
            image_size,
            render_time,
            sample_rate
        );

        if (sample >= samples || input.r) {
            if (image_rw::write_png(sampler.get_image(), "render_result.png") == 0)
                return_code = 1;
            break;
        }

        this_t = glfwGetTime();
        delta_t = this_t - last_t;
        last_t = this_t;
        render_time = this_t - start_t;
        sample_rate = (float)sample / render_time;
    }

    sampler.destroy_image();

    return return_code;
}

} /* trc */

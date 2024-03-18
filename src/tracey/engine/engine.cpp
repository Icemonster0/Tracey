#include "engine.hpp"

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <memory>
#include <chrono>

#include "../../lib/glfw.hpp"
#include "../util/math_util.hpp"
#include "input_package.hpp"
#include "../util/parser.hpp"
#include "../geometry/shape.hpp"
#include "../geometry/shapes/shapes.hpp"
#include "../graphics/image_read_write.hpp"
#include "../graphics/brdfs.hpp"
#include "../graphics/color_spaces.hpp"

namespace trc {

Engine::Engine(UserConfig cfg) : cfg(cfg), error(0), preview_mode(true) {}

int Engine::load_file(std::string file_path) {
    if (cfg.color_mode == "STANDARD") {
        color::color_mode = color::STANDARD;
    } else if (cfg.color_mode == "FILMIC") {
        color::color_mode = color::FILMIC;
    } else if (cfg.color_mode == "RAW") {
        color::color_mode = color::RAW;
    } else {
        printf("\n%s is not a valid color mode (must be one of STANDARD, FILMIC, RAW)\n", cfg.color_mode.c_str());
        return 5;
    }

    if(system("clear")) {};
    printf("Loading file '%s'\n", file_path.c_str());

    if (file_path.size() >= 7 && file_path.substr(file_path.size() - 7) == ".tracey") {
        // native file: use own parser
        TraceyParser parser;
        if (!parser.load_file(file_path, &shader_pack)) {
            printf("\nImport error:\n%s\n\n", parser.get_error_string().c_str());
            return 1;
        }
        scene = parser.get_loaded_scene();
    }
    else {
        // non-native file: use assimp
        Importer importer;
        if (!importer.load_file(file_path, &shader_pack)) {
            printf("\nImport error:\n%s\n\n", importer.get_error_string().c_str());
            return 1;
        }
        scene = importer.get_loaded_scene();
    }

    if (cfg.accelerator == "NONE") {
        accelerator = std::unique_ptr<Accelerator> {new Accelerator {&scene}};
    } else if (cfg.accelerator == "BVH") {
        accelerator = std::unique_ptr<Accelerator> {new BVH {&scene}};
    } else if (cfg.accelerator == "VOXEL") {
        accelerator = std::unique_ptr<Accelerator> {new VoxelAccel {&scene, cfg.voxel_size}};
    } else {
        printf("\n%s is not a valid accelerator type (must be one of NONE, BVH, VOXEL)\n", cfg.accelerator.c_str());
        return 4;
    }

    printf("\n");
    return 0;
}

int Engine::run() {
    // Tests
    // test_scene_setup();
    // test_integrals();

    window_manager = WindowManager {cfg.window_size};
    viewer = Viewer {window_manager.get_size(), cfg.fov, cfg.focal_length, cfg.aperture};
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
            accelerator.get(),
            &shader_pack,
            seed_gen(),
            window_manager.is_update_required(),
            cfg.samples,
            preview_mode,
            cfg.exposure,
            cfg.bounces
        );
        window_manager.draw_frame(sampler.get_frame_buffer());
        InputPackage input = window_manager.handle_events();
        viewer.update(input, delta_t, window_manager.get_size());

        if (input.r) error = render_image(&seed_gen, false);
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
            viewer.get_speed(),
            viewer.get_camera()->get_focal_length(),
            viewer.get_camera()->get_aperture()
        );

        this_frame_t = glfwGetTime();
        delta_t = this_frame_t - last_frame_t;
        last_frame_t = this_frame_t;
    }

    window_manager.close();
    console.clear();
    return error;
}

int Engine::render() {
    sampler = Sampler {window_manager.get_size()};

    std::random_device rand_dev;
    std::mt19937 seed_gen {rand_dev()};

    error = render_image(&seed_gen, true);
    if (error == 0) printf("Done!\n\n");

    return error;
}

int Engine::render_image(std::mt19937 *seed_gen, bool render_only) {
    // render_only: rendering without graphical context,
    // which means there is no input either

    console.clear();

    InputPackage input;
    if (!render_only) input = window_manager.handle_events();

    Camera render_camera;
    if (render_only) {
        std::optional<Camera> maybe_camera = *scene.get_camera();
        if (maybe_camera) {
            render_camera = maybe_camera.value();
        } else {
            printf("The scene contains no camera, aborting render\n");
            return 9;
        }
    }
    else {
        render_camera = *viewer.get_camera();
    }
    render_camera.set_aspect((float)cfg.render_size.x / (float)cfg.render_size.y);

    int return_code = 0;

    sampler.initialize_image(cfg.render_size);

    std::chrono::high_resolution_clock timer;
    auto start_t = timer.now();
    auto last_t = timer.now();
    auto this_t = timer.now();
    float delta_t = 0.f;
    float render_time = 0.f;
    float sample_rate = 1.f;

    int sample = 0;
    while (true) {
        if (!render_only && (input.i || window_manager.window_should_close())) {
            break;
        }

        console.print_render_info(
            cfg.console_frequency,
            delta_t,
            sample,
            cfg.samples,
            cfg.render_size,
            render_time,
            sample_rate,
            render_only
        );

        ++sample;
        sampler.render_image_sample(
            &render_camera,
            accelerator.get(),
            &shader_pack,
            seed_gen->operator()(),
            sample,
            cfg.exposure,
            cfg.bounces
        );

        if (!render_only) input = window_manager.handle_events();

        if (sample >= cfg.samples || (!render_only && input.r)) {
            if (image_rw::write_png(sampler.get_image(), cfg.output_path) == 0)
                return_code = 1;
            if (sample >= cfg.samples) break;
        }

        this_t = timer.now();
        delta_t = std::chrono::duration_cast<std::chrono::microseconds>(this_t - last_t).count() / 1000000.f;
        last_t = this_t;
        render_time = std::chrono::duration_cast<std::chrono::microseconds>(this_t - start_t).count() / 1000000.f;
        sample_rate = (float)sample / render_time;
    }

    sampler.destroy_image();

    return return_code;
}

// Tests

void Engine::test_scene_setup() {
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


    /* MESH TEST SETUP */
    // std::vector<glm::vec3> pos_vec {
    //     {-0.5f, -0.5f, 0.0f},
    //     {0.5f, -0.5f, 0.0f},
    //     {0.0f, 0.5f, 0.0f}
    // };
    // std::vector<glm::vec3> normal_vec {
    //     {0.f, 0.f, 1.f},
    // };
    // std::vector<glm::vec2> tex_vec {
    //     {0.0f, 0.0f},
    //     {1.0f, 0.0f},
    //     {0.5f, 1.0f}
    // };
    // Mesh *mesh = scene.add_mesh(std::make_unique<Mesh>(pos_vec, normal_vec, tex_vec));
    //
    // scene.add_light(std::unique_ptr<Light>(new PointLight(glm::vec3 {3.f, 3.f, 3.f}, glm::vec3 {1.f}, 30.f, 2.5f)));
    //
    // Material *floor_mat = scene.add_material(std::make_unique<Material>(glm::vec3 {1.f}, 1.f, 0.f));
    // Material *tri_mat = scene.add_material(std::make_unique<Material>(glm::vec3 {1.f, 0.4f, 0.05f}, 0.f, 0.7f));
    //
    // scene.add_object(std::unique_ptr<Shape>(new GroundPlane(-0.5f, shader_pack.shader_combined.get(), floor_mat)));
    // scene.add_object(std::unique_ptr<Shape>(new Triangle({0, 1, 2}, {0, 0, 0}, {0, 1, 2}, mesh, shader_pack.shader_combined.get(), tri_mat)));
}

void Engine::test_integrals() {
    const float step = 0.001f;
    const float phi_max = 2.f*3.141592654f;
    const float theta_max = 3.141592654f;

    const float roughness = 0.3f;
    const glm::vec3 normal {0, 1, 0};
    const glm::vec3 tangent {1, 0, 0};

    std::random_device rand_dev;
    unsigned long seed = rand_dev();
    RNG rng {seed, seed << 2, (seed >> 12) + 0xabc, seed * 3 >> 31};

    printf("Calculating Integrals ...\n\n");

    float integral_0 = 0.f;
    float integral_1 = 0.f;
    float integral_2 = 0.f;
    for (float phi = 0.f; phi < phi_max; phi += step) {
        for (float theta = 0.f; theta < theta_max; theta += step) {
            Ray ray {
                glm::vec3 {0},
                glm::vec3 {
                    sin(theta) * cos(phi),
                    cos(theta),
                    sin(theta) * sin(phi)
                }
            };
            const float sum_factor = abs(sin(phi)) * step*step;

            float value_0 = accelerator->calc_light_intersection(ray).light.r;
            value_0 *= sum_factor;
            integral_0 += value_0;

            // ggx distrib
            Ray ggx_ray {
                glm::vec3 {0},
                glm::reflect(ray.direction, brdf::ggx_normal(
                    normal,
                    tangent,
                    roughness,
                    &rng
                ))
            };
            float value_1 = accelerator->calc_light_intersection(ggx_ray).light.r;
            value_1 *= sum_factor;
            integral_1 += value_1;

            float value_2 = accelerator->calc_light_influence(
                glm::vec3 {0},
                normal,
                ray.direction,
                roughness,
                &rng,
                brdf::ggx
            ).r;

            // ggx begin
            // float a = roughness*roughness;
            // float c = glm::dot(normal, math::normalize(ray.direction + ray.direction));
            // float value_2 = a / (c*c * (a*a - 1.f) + 1.f);
            // value_2 = value_2*value_2 * (1 / 3.141592654f) * a;
            // ggx end

            // float value_2 = 1.f;
            // float value_2 = brdf::ggx(ray.direction, ray.direction, normal, roughness);
            value_2 *= sum_factor;
            integral_2 += value_2;

            // printf("0: %f\n", value_0);
            // printf("1: %f\n", value_1);
            // printf("2: %f\n\n", value_2);
        }
        // printf("2: %f\n\n", integral_2);
    }
    printf("(0: reference; 1: ggx indirect; 2: ggx direct)\n");
    printf("integral 0 = %f\n", integral_0);
    printf("integral 1 = %f\n", integral_1);
    printf("integral 2 = %f , %f\n", integral_2, roughness*roughness);
    // printf("integral 2 = %f * %f = %f\n", integral_2, roughness*roughness, roughness*roughness * integral_2);
    exit(0);
}

} /* trc */

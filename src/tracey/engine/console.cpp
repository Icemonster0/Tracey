#include "console.hpp"

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <iomanip>

#define PADDED std::setw(10)
#define DECIMAL std::fixed<<std::setprecision(2)

namespace trc {

Console::Console() : print_cooldown(0.f) {}

void Console::print(float frequency, float delta_t, bool preview_mode, int samples,
        int max_samples, glm::ivec2 window_size, glm::vec3 view_pos, float yaw,
        float pitch, float fov, float speed, float focal_length, float aperture) {

    print_cooldown -= delta_t;
    if (print_cooldown <= 0.f) {
        std::stringstream s;

        s << "--- Tracey Engine ---" << '\n';
        s << '\n';

        s << "Info" << '\n';
        s << " mode:         " << PADDED << (preview_mode ? "Preview" : "Rendered") << '\n';
        s << " fps:          " << PADDED << int(1.f / delta_t) << '\n';
        s << " samples:      " << PADDED << samples << " / " << max_samples << '\n';
        s << " frame size:   " << PADDED << window_size.x << "x" << window_size.y << '\n';
        s << " camera pos:   " << PADDED << DECIMAL << view_pos.x << " " << PADDED << DECIMAL << view_pos.y << " " << PADDED << DECIMAL << view_pos.z << '\n';
        s << " camera rot:   " << PADDED << DECIMAL << yaw << " " << PADDED << DECIMAL << pitch << '\n';
        s << " camera fov:   " << PADDED << int(fov) << '\n';
        s << " camera speed: " << PADDED << DECIMAL << speed << '\n';
        s << " focal length: " << PADDED << DECIMAL << focal_length << '\n';
        s << " aperture:     " << PADDED << DECIMAL << aperture << '\n';
        s << '\n';

        s << "Controls" << '\n';
        s << " ESC:          Quit" << '\n';
        s << " ENTER:        Toggle mouse focus" << '\n';
        s << " W, A, S, D:   Movement (fly navigation)" << '\n';
        s << " E, Q:         Move up, move down (vertical)" << '\n';
        s << " SCROLL:       Adjust fly speed" << '\n';
        s << " R:            Render image to output-path" << '\n';
        s << " P:            Toggle preview / rendered mode" << '\n';

        clear();
        printf("%s\n", s.str().c_str());

        print_cooldown = 1.f / frequency;
    }
}

void Console::print_render_info(float frequency, float delta_t, int sample,
        int max_samples, glm::ivec2 image_size, float time, float sample_rate,
        bool render_only) {

    print_cooldown -= delta_t;
    if (print_cooldown <= 0.f) {
        std::stringstream s;

        float percentage = (float)sample / (float)max_samples * 100.f;

        int t_hours = time / 3600;
        int t_minutes = (time - t_hours * 3600) / 60;
        int t_seconds = (time - t_hours * 3600.f) - t_minutes * 60.f;

        float time_left = (max_samples - sample) / sample_rate;
        int tl_hours = time_left / 3600;
        int tl_minutes = (time_left - tl_hours * 3600) / 60;
        int tl_seconds = (time_left - tl_hours * 3600.f) - tl_minutes * 60.f;

        s << "--- Tracey Render ---" << '\n';
        s << '\n';

        s << "Info" << '\n';
        s << " image size:   " << PADDED << image_size.x << "x" << image_size.y << '\n';
        s << " samples:      " << PADDED << sample << " / " << max_samples << " (" << DECIMAL << percentage << "%)" << '\n';
        s << " time:         " << PADDED;
        if (t_hours) s << t_hours << "h ";
        if (t_minutes) s << t_minutes << "m ";
        if (t_seconds) s << t_seconds << "s";
        s << '\n';
        s << " time left:    " << PADDED;
        if (tl_hours) s << tl_hours << "h ";
        if (tl_minutes) s << tl_minutes << "m ";
        if (tl_seconds) s << tl_seconds << "s";
        s << '\n';

        if (!render_only) {
            s << '\n';
            s << "Controls" << '\n';
            s << " ESC:          Quit" << '\n';
            s << " ENTER:        Toggle mouse focus" << '\n';
            s << " R:            Save rendered image (does not interrupt)" << '\n';
            s << " I:            Interrupt render (does not save)" << '\n';
        }

        clear();
        printf("%s\n", s.str().c_str());

        print_cooldown = 1.f / frequency;
    }
}

void Console::clear() {
    if(system("clear")) {};
}

#undef PADDED
#undef TIME
#undef DECIMAL

} /* trc */

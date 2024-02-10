#include "console.hpp"

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <iomanip>

namespace trc {

Console::Console() : print_cooldown(0.f) {}

void Console::print(float frequency, float delta_t, int samples, int max_samples, glm::ivec2 window_size, glm::vec3 view_pos, float yaw, float pitch, float fov, float speed) {
    if (print_cooldown <= 0.f) {
        std::stringstream s;

#define PADDED std::setw(10)<<
#define DECIMAL std::fixed<<std::setprecision(2)<<

        s << "--- Tracey Engine ---" << '\n';
        s << '\n';

        s << "Info" << '\n';
        s << " fps:          " << PADDED int(1.f / delta_t) << '\n';
        s << " samples:      " << PADDED samples << " / " << max_samples << '\n';
        s << " frame width:  " << PADDED window_size.x << '\n';
        s << " frame height: " << PADDED window_size.y << '\n';
        s << " camera pos:   " << PADDED DECIMAL view_pos.x << " " << PADDED DECIMAL view_pos.y << " " << PADDED DECIMAL view_pos.z << '\n';
        s << " camera rot:   " << PADDED DECIMAL yaw << " " << PADDED DECIMAL pitch << '\n';
        s << " camera fov:   " << PADDED int(fov) << '\n';
        s << " camera speed: " << PADDED DECIMAL speed << '\n';
        s << '\n';

        s << "Controls" << '\n';
        s << " ESC:          Quit" << '\n';
        s << " ENTER:        Toggle mouse focus" << '\n';
        s << " W, A, S, D:   Movement (fly navigation)" << '\n';
        s << " E, Q:         Move up, move down (vertical)" << '\n';
        s << " SCROLL:       Adjust fly speed" << '\n';

#undef PADDED
#undef DECIMAL

        system("clear");
        printf("%s\n", s.str().c_str());

        print_cooldown = 1.f / frequency;
    }
    else {
        print_cooldown -= delta_t;
    }
}

void Console::close() {
    system("clear");
}

} /* trc */

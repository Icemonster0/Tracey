#include "console.hpp"

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <iomanip>

namespace trc {

Console::Console() {}

Console::Console(float refresh_rate)
    : frequency(refresh_rate), print_cooldown(0.f) {}

void Console::print(float delta_t, glm::ivec2 window_size) {
    if (print_cooldown <= 0.f) {
        std::stringstream s;

#define PADDED std::setw(5)<<

        s << "--- Tracey Engine ---" << '\n';
        s << '\n';

        s << "Info" << '\n';
        s << " fps:          " << PADDED int(1.f / delta_t) << '\n';
        s << " frame width:  " << PADDED window_size.x << '\n';
        s << " frame height: " << PADDED window_size.y << '\n';
        s << '\n';

        s << "Controls" << '\n';
        s << " ESC:          Quit" << '\n';

#undef PADDED

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

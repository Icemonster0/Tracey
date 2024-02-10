#ifndef USER_CONFIG_HPP
#define USER_CONFIG_HPP

#include <string>
#include <vector>

#include "../../lib/clom.hpp"
#include "../../lib/glm.hpp"

namespace trc {

namespace cfg_parser {
    glm::ivec2 str_to_ivec2(std::string str);
} /* cfg_parser */

struct UserConfig {
    CLOM2_SETTING_FLOAT(console_frequency, --console-frequency, -cf, 5.0, Refresh rate for the console interface in hertz);
    CLOM2_SETTING_FLOAT(fov, --fov, -fov, 70.0, Vertical field of view of the viewer camera in degrees);
    CLOM2_SETTING_INT(samples, --samples, -s, 4096, Maximum number of samples rendered in the viewport);
    CLOM2_GENERAL_SETTING(window_size, --window-size, -ws, glm::ivec2, 400 300, Width and height of the initial window in pixels, trc::cfg_parser::str_to_ivec2);
};

} /* trc */

#endif /* end of include guard: USER_CONFIG_HPP */

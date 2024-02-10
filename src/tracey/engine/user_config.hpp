#ifndef USER_CONFIG_HPP
#define USER_CONFIG_HPP

#include <string>
#include <vector>

#include "../../clom.hpp"
#include "../../glm.hpp"

namespace trc {

namespace cfg_parser {
    glm::ivec2 str_to_ivec2(std::string str);
} /* cfg_parser */

struct UserConfig {
    CLOM2_GENERAL_SETTING(window_size, --window-size, -ws, glm::ivec2, 400 300, Width and height in pixels of the initial window, trc::cfg_parser::str_to_ivec2);
};

} /* trc */

#endif /* end of include guard: USER_CONFIG_HPP */

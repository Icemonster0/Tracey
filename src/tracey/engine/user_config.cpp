#include "user_config.hpp"

namespace trc {

namespace cfg_parser {
    glm::ivec2 str_to_ivec2(std::string str) {
        std::vector<int> int_vec = clom2::str_to_int_vec(str);
        if (int_vec.size() == 0) {
            return glm::ivec2(0.f);
        } else if (int_vec.size() == 1) {
            return glm::ivec2(int_vec[0]);
        } else {
            return glm::ivec2(int_vec[0], int_vec[1]);
        }
    }
} /* cfg_parser */

} /* trc */

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
    CLOM2_SETTING_STRING(accelerator, --accelerator, -ac, BVH, Accelerator structure to use (one of NONE BVH VOXEL));
    CLOM2_SETTING_FLOAT(aperture, --aperture, -ap, 0.0, Aperture radius of the viewer came);
    CLOM2_SETTING_STRING(color_mode, --color-mode, -cm, FILMIC, Color management mode (one of STANDARD, FILMIC, RAW));
    CLOM2_SETTING_FLOAT(console_frequency, --console-frequency, -cf, 5.0, Refresh rate for the console interface in hertz);
    CLOM2_SETTING_FLOAT(exposure, --exposure, -ex, 0.0, The exposure to apply to the rendered image);
    CLOM2_SETTING_FLOAT(focal_length, --focal-length, -fl, 10.0, Focus distance of the viewer camera);
    CLOM2_SETTING_FLOAT(fov, --fov, -fov, 70.0, Vertical field of view of the viewer camera in degrees);
    CLOM2_SETTING_STRING(output_path, --output-path, -o, ./render-result.png, Path where the rendered image is saved including file name (must be a png));
    CLOM2_GENERAL_SETTING(render_size, --render-size, -rs, glm::ivec2, 1280 720, Resolution of the rendered image, trc::cfg_parser::str_to_ivec2);
    CLOM2_SETTING_INT(samples, --samples, -s, 4096, Maximum number of samples rendered in the viewport and the rendered image);
    CLOM2_SETTING_STRING(scene_path, --scene, -S, , Path to a 3d model or scene file);
    CLOM2_SETTING_FLOAT(voxel_size, --voxel-size, -vs, 0.1, Size of the voxels when using VOXEL accelerator (see --accelerator));
    CLOM2_GENERAL_SETTING(window_size, --window-size, -ws, glm::ivec2, 400 300, Width and height of the initial window in pixels, trc::cfg_parser::str_to_ivec2);
    CLOM2_FLAG(render, --render, -r, Render image directly using scene camera (skips interactive interface));
};

} /* trc */

#endif /* end of include guard: USER_CONFIG_HPP */

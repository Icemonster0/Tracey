#include <cstdio>
#include <filesystem>

#include "tracey/engine/engine.hpp"
#include "tracey/engine/user_config.hpp"
#include "lib/clom.hpp"

int main(int argc, char const *argv[]) {

    // Parse command line configuration
    CLOM2_SET_ARGS(argc, argv);
    CLOM2_CHECK_FOR_HELP_BEGIN(help, h);
    trc::UserConfig cfg;
    CLOM2_CHECK_FOR_HELP_END();

    // Create the engine
    trc::Engine engine {cfg};
    int result = 0;

    // Process output directory string
    std::size_t last_sep = cfg.output_path.find_last_of("\\/");
    std::string out_dir = (last_sep == std::string::npos) ? "." : cfg.output_path.substr(0, last_sep);

    // If certain options are incorrect, abort
    if (result = engine.validate_options()) {}
    // If output directory doesn't exist, abort
    else if (!std::filesystem::is_directory(std::filesystem::path(out_dir))) {
        printf("Directory doesn't exist: %s\n", out_dir.c_str());
        result = 7;
    }
    // If no scene file is specified, abort
    else if (cfg.scene_path.length() == 0) {
        printf("Usage: Tracey -S path/to/scene/file\n");
        return 1;
    }
    else {
        // Load scene file
        result = engine.load_file(cfg.scene_path);

        // If successful, start the graphical runtime or render image
        if (result == 0) {
            if (cfg.render) {
                result = engine.render(); // render only
            } else {
                result = engine.run(); // graphical runtime
            }
        }
    }

    // Exit once the user quits or a crash occurs
    printf("Engine exited with code %d\n", result);

    return result;
}

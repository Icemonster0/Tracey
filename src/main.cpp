#include <cstdio>

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

    // Start the graphical runtime
    int result = engine.run();

    // Exit once the user quits or a crash occurs
    printf("Engine exited with code %d\n", result);

    return result;
}

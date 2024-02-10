#include <cstdio>

#include "tracey/engine/engine.hpp"
#include "tracey/engine/user_config.hpp"
#include "clom.hpp"

int main(int argc, char const *argv[]) {

    CLOM2_SET_ARGS(argc, argv);
    CLOM2_CHECK_FOR_HELP_BEGIN(help, h);
    trc::UserConfig cfg;
    CLOM2_CHECK_FOR_HELP_END();

    trc::Engine engine {cfg};

    int result = engine.run();

    printf("Engine exited with code %d\n", result);

    return result;
}

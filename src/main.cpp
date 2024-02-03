#include <cstdio>

#include "tracey/engine/engine.hpp"

int main(int argc, char const *argv[]) {

    trc::Engine engine {};

    int result = engine.run();

    printf("Engine exited with code %d\n", result);

    return result;
}

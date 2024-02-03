cmake_minimum_required(VERSION 3.8)
project(Tracey VERSION 1.0.0)

add_subdirectory(lib/glfw-3.3.8)

add_library(main_module STATIC
    src/glfw.hpp
    src/glm.hpp
    src/tracey/engine/console.cpp
    src/tracey/engine/console.hpp
    src/tracey/engine/engine.cpp
    src/tracey/engine/engine.hpp
    src/tracey/engine/sampler.cpp
    src/tracey/engine/sampler.hpp
    src/tracey/engine/viewer.cpp
    src/tracey/engine/viewer.hpp
    src/tracey/engine/window_manager.cpp
    src/tracey/engine/window_manager.hpp
    src/tracey/geometry/camera.cpp
    src/tracey/geometry/camera.hpp
    src/tracey/graphics/buffer.hpp
    src/tracey/scene/scene.cpp
    src/tracey/scene/scene.hpp
)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"))
    message("Darwin Clang detected")
    target_compile_options(main_module BEFORE PUBLIC -Xpreprocessor -fopenmp -O3)
else()
    target_compile_options(main_module BEFORE PUBLIC -fopenmp -O3)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(main_module pthread)
endif()

target_link_libraries(main_module glfw GL)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} main_module)

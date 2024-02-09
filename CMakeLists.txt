cmake_minimum_required(VERSION 3.8)
project(Tracey VERSION 1.0.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_subdirectory(lib/glfw-3.3.8)

add_library(main_module STATIC
    src/glfw.hpp
    src/glm.hpp
    src/tracey/engine/accelerator.cpp
    src/tracey/engine/accelerator.hpp
    src/tracey/engine/console.cpp
    src/tracey/engine/console.hpp
    src/tracey/engine/engine.cpp
    src/tracey/engine/engine.hpp
    src/tracey/engine/sampler.cpp
    src/tracey/engine/sampler.hpp
    src/tracey/engine/scene.cpp
    src/tracey/engine/scene.hpp
    src/tracey/engine/viewer.cpp
    src/tracey/engine/viewer.hpp
    src/tracey/engine/window_manager.cpp
    src/tracey/engine/window_manager.hpp
    src/tracey/geometry/lights/lights.hpp
    # src/tracey/geometry/lights/point_light.cpp TODO
    # src/tracey/geometry/lights/point_light.hpp TODO
    src/tracey/geometry/shapes/ground_plane.cpp
    src/tracey/geometry/shapes/ground_plane.hpp
    src/tracey/geometry/shapes/shapes.hpp
    src/tracey/geometry/shapes/sphere.cpp
    src/tracey/geometry/shapes/sphere.hpp
    src/tracey/geometry/camera.cpp
    src/tracey/geometry/camera.hpp
    src/tracey/geometry/intersection.cpp
    src/tracey/geometry/intersection.hpp
    src/tracey/geometry/light.cpp
    src/tracey/geometry/light.hpp
    src/tracey/geometry/ray.cpp
    src/tracey/geometry/ray.hpp
    src/tracey/geometry/shape.cpp
    src/tracey/geometry/shape.hpp
    src/tracey/graphics/shaders/shader_syntax.hpp
    src/tracey/graphics/shaders/shaders.cpp
    src/tracey/graphics/shaders/shaders.hpp
    src/tracey/graphics/buffer.hpp
    src/tracey/graphics/shader_data.hpp
    src/tracey/graphics/shader_pack.cpp
    src/tracey/graphics/shader_pack.hpp
    src/tracey/graphics/shader.cpp
    src/tracey/graphics/shader.hpp
    src/tracey/random/random.hpp
    src/tracey/random/xoshiro256plus.cpp
    src/tracey/random/xoshiro256plus.hpp
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

target_link_libraries(main_module glfw GL -fopenmp)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} main_module)

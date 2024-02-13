# Tracey
A minimal progressive CPU raytracer.  (Work in progress)

![render example](media/render_example.png)

## Building
To compile the source code, use CMake:
```
cmake -S path/to/Tracey -B path/to/build/dir
cd path/to/build/dir
make
```
where `path/to/Tracey` should be the directory in which `CMakeLists.txt` can be found.  

To be able to compile, you need
- CMake
- a C++ compiler with C++17 support
- GL library
- openmp library

(The latter two of which are usually installed (on Linux) if you have g++ (I think))  

To run, execute the generated binary from the terminal (it can also be run without a terminal, but it has a nice console interface so it is recommended).

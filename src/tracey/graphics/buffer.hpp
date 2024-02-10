#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "../../lib/glm.hpp"

#include <cstdlib>

namespace trc {

template <typename T>
class Buffer {
public:
    Buffer(glm::ivec2 buf_size, T value) {
        fill(buf_size, value);
    }

    Buffer(glm::ivec2 buf_size) {
        fill(buf_size, T {});
    }

    Buffer() {}

    Buffer(const Buffer &source) : size(source.size) {
        free(data);

        unsigned int n_bytes = sizeof(T) * size.x * size.y;
        data = (T*)malloc(n_bytes);
        memcpy(data, source.data, n_bytes);
    }

    Buffer &operator=(const Buffer &source) {
        size = source.size;

        free(data);

        unsigned int n_bytes = sizeof(T) * size.x * size.y;
        data = (T*)malloc(n_bytes);
        memcpy(data, source.data, n_bytes);

        return *this;
    }

    Buffer(Buffer &&source) : size(source.size), data(source.data) {
        source.data = nullptr;
    }

    Buffer &operator=(Buffer &&source) {
        size = source.size;
        data = source.data;
        source.data = nullptr;
        return *this;
    }

    ~Buffer() {
        free(data);
    }

    void fill(glm::ivec2 buf_size, T value) {
        size = buf_size;

        unsigned int n_bytes = sizeof(T) * size.x * size.y;
        data = (T*)realloc(data, n_bytes);

        fill(value);
    }

    void fill(T value) {
        const int length = size.x * size.y;
        for (int i = 0; i < length; ++i) {
            data[i] = value;
        }
    }

    glm::ivec2 get_size() const {
        return size;
    }

    T *at(glm::ivec2 index) {
        return data + (index.y * size.x + index.x);
    }

    const T *get_data() const {
        return data;
    }

private:
    glm::ivec2 size;
    T *data = nullptr;
};

} /* trc */

#endif /* end of include guard: BUFFER_HPP */

#include "color_spaces.hpp"

namespace trc::color {

ColorMode color_mode = FILMIC;

glm::vec3 raw_to_sRGB(glm::vec3 color) {
    const float c = 1.f / 2.2f;
    return glm::vec3 {
        powf(color.x, c),
        powf(color.y, c),
        powf(color.z, c)
    };
}
float raw_to_sRGB(float color) {
    const float c = 1.f / 2.2f;
    return powf(color, c);
}

glm::vec3 sRGB_to_raw(glm::vec3 color) {
    const float c = 2.2f;
    return glm::vec3 {
        powf(color.x, c),
        powf(color.y, c),
        powf(color.z, c)
    };
}
float sRGB_to_raw(float color) {
    const float c = 2.2f;
    return powf(color, c);
}

const glm::mat3 ACEScg_to_raw_mat = {
    1.7048873310f, -0.1295209353f, -0.0241270599f,
   -0.6241572745f,  1.1383993260f, -0.1246206123f,
   -0.0808867739f, -0.0087792418f,  1.1488221099f
};

glm::vec3 ACEScg_to_raw(glm::vec3 color) {
    return ACEScg_to_raw_mat * color;
}
float ACEScg_to_raw(float color) {
    return color;
}

const glm::mat3 raw_to_ACEScg_mat = {
    0.6131178129f, 0.0699340823f, 0.0204629926f,
    0.3411819959f, 0.9181030375f, 0.1067686634f,
    0.0457873443f, 0.0119327755f, 0.8727159106f
};

glm::vec3 raw_to_ACEScg(glm::vec3 color) {
    return raw_to_ACEScg_mat * color;
}
float raw_to_ACEScg(float color) {
    return color;
}

// transform for ACEScg to filmic view
float T(float x) {
    // parameters
    float S = 1.f; // scale
    float ga = 0.8f; // slope
    float t0 = 0.55f; // toe
    float t1 = 0.f; // black clip
    float s0 = 0.26f; // shoulder
    float s1 = 0.04f; // white clip

    float c = log10(x);

    float ta = (1.f - t0 - 0.18f) / ga - 0.733f;
    float sa = (s0 - 0.18f) / ga - 0.733f;

    if (ta < c && c < sa) { // middle part
        return S * (ga * (c + 0.733f) + 0.18f);
    }

    float mid = (ta + sa) / 2.f;

    if (c < mid) { // tail part
        float num = 1.f + t1 - t0;
        float numerator = 2.f * num;
        float exponent = ((-2.f * ga) / num) * (c - ta);
        return S * (numerator / (1.f + exp(exponent)) - t1);
    }
    else { // shoulder part
        float num = 1.f + s1 - s0;
        float numerator = 2.f * num;
        float exponent = ((2.f * ga) / num) * (c - sa);
        return S * (1.f + s1 - numerator / (1.f + exp(exponent)));
    }
}
glm::vec3 ACEScg_to_filmic(glm::vec3 color) {
    color.r = T(color.r);
    color.g = T(color.g);
    color.b = T(color.b);
    return color;
}
float ACEScg_to_filmic(float color) {
    return T(color);
}

} /* trc::color */

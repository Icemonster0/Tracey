#include "math_util.hpp"

#include <algorithm>

namespace trc::math {

float fast_inv_sqrt(float number) {
    union {
        float f;
        uint32_t i;
    } conv;

    float x2;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    conv.f = number;
    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f = conv.f * (threehalfs - (x2 * conv.f * conv.f));
    return conv.f;
}

glm::vec3 normalize(glm::vec3 vec) {
    // Not using fast_inv_sqrt because it is inaccurate and does
    // not improve performance
    return glm::normalize(vec);
    // return vec * fast_inv_sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

glm::vec3 random_dir_in_hemisphere(glm::vec3 normal, RNG *rng) {
    // Too slow:
    // std::normal_distribution distrib;
    //
    // glm::vec3 dir {
    //     distrib(*rng),
    //     distrib(*rng),
    //     distrib(*rng)
    // };

    std::uniform_real_distribution<float> distrib {0.f, 1.f};

    // Good:
    glm::vec3 dir {
        normsinv(distrib(*rng)),
        normsinv(distrib(*rng)),
        normsinv(distrib(*rng))
    };

    dir *= glm::sign(glm::dot(dir, normal));

    return normalize(dir);
}

float fresnel(float n1, float n2, glm::vec3 incident, glm::vec3 normal) {
    /**
     * Method from pbrt:
     * Copyright (c) 1998-2015, Matt Pharr, Greg Humphreys, and Wenzel Jakob.
     * All rights reserved.
     * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
     * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
     * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    */

    float cos_theta_1 = std::clamp(glm::dot(incident, normal), -1.f, 1.f);

    // Potentially swap indices of refraction
    bool entering = cos_theta_1 > 0.f;
    if (!entering) {
        std::swap(n1, n2);
        cos_theta_1 = std::abs(cos_theta_1);
    }

    // Compute cos_theta_2 using Snell’s law
    float sin_theta_1 = std::sqrt(std::max(0.f, 1.f - cos_theta_1 * cos_theta_1));
    float sin_theta_2 = n1 / n2 * sin_theta_1;

    // Handle total internal reflection
    if (sin_theta_2 >= 1)
        return 1;

    float cos_theta_2 = std::sqrt(std::max(0.f, 1.f - sin_theta_2 * sin_theta_2));

    float Rparl = ((n2 * cos_theta_1) - (n1 * cos_theta_2)) /
                  ((n2 * cos_theta_1) + (n1 * cos_theta_2));
    float Rperp = ((n1 * cos_theta_1) - (n2 * cos_theta_2)) /
                  ((n1 * cos_theta_1) + (n2 * cos_theta_2));
    return (Rparl * Rparl + Rperp * Rperp) / 2;
}

std::optional<float> intersect_sphere(Ray ray, glm::vec3 center, float radius) {
    // translate ray to object space
    ray.origin -= center;

    // coefficients for the quadratic equation at² + bt + c = 0
    const float a = ray.direction.x*ray.direction.x + ray.direction.y*ray.direction.y + ray.direction.z*ray.direction.z;
    const float b = 2 * (ray.direction.x*ray.origin.x + ray.direction.y*ray.origin.y + ray.direction.z*ray.origin.z);
    const float c = ray.origin.x*ray.origin.x + ray.origin.y*ray.origin.y + ray.origin.z*ray.origin.z - radius*radius;

    // solve equation
    const float discriminant = b*b - 4*a*c;

    if (discriminant < 0.f) return std::optional<float>(); // no solution

    const float inv_denominator = 1.f / (2*a);

    float t;

    if (discriminant == 0) { // one solution
        t = -b * inv_denominator;
        if (t < TRC_RAY_CLIP_EPSILON) return std::optional<float>();
    }
    else { // two solutions
        float sqrt_discriminant = sqrtf(discriminant);
        float t1 = (-b + sqrt_discriminant) * inv_denominator;
        float t2 = (-b - sqrt_discriminant) * inv_denominator;
        if (t1 < TRC_RAY_CLIP_EPSILON) {
            if (t2 < TRC_RAY_CLIP_EPSILON) return std::optional<float>();
            else t = t2;
        } else if (t2 < TRC_RAY_CLIP_EPSILON) t = t1;
        else t = std::min(t1, t2);
    }

    return std::optional<float>(t);
}

float quad_interp(float t) {
    return (t < 0.5f) ?
           (2.f * t*t) :
           (-2.f * (t-1.f)*(t-1.f) + 1.f);
}

float cube_interp(float t) {
    float t_minus_one = (t-1.f);
    return (t < 0.5f) ?
           (4.f * t*t*t) :
           (4.f * t_minus_one*t_minus_one*t_minus_one + 1.f);
}

} /* trc::math */

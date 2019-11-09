#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "hittable.h"
#include "random.h"
#include <cmath>

vec3 reflect(const vec3 &v, const vec3 &n) {
    return v - 2 * dot(v, n) * n;
}

bool refract(const vec3 &v, const vec3 &n, float ni_over_no, vec3 &refracted) {
    vec3 uv = unit_vector(v);
    float cos1 = -dot(uv, n);
    float discriminant = 1.0 - ni_over_no * ni_over_no * (1 - cos1 * cos1);
    if (discriminant > 0) {
        refracted = ni_over_no * (uv + n * cos1) - n * sqrt(discriminant);
        return true;
    }
    else // total internal reflection
        return false;
}

class material {
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const = 0;
    virtual vec3 emitted(float u, float v, const vec3 &p) const {
        return vec3(0, 0, 0);
    }
};

#endif
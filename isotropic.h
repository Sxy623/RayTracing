#ifndef ISOTROPIC_H
#define ISOTROPIC_H

#include "material.h"
#include "texture.h"

class isotropic: public material {
public:
    isotropic(texture *a): albedo(a) {}

    virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const {
        scattered = ray(rec.p, random_in_unit_sphere(), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

    texture *albedo;
};

#endif
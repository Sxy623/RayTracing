#ifndef ROTATE_H
#define ROTATE_H

#include "hittable.h"
#include <cmath>
#include <cfloat>

class rotate_y: public hittable {
public:
    rotate_y(hittable *p, float angle);

    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const;
    virtual bool bounding_box(float t0, float t1, aabb &box) const {
        box = bbox;
        return hasbox;
    }

    hittable *ptr;
    float sin_theta;
    float cos_theta;
    bool hasbox;
    aabb bbox;
};

rotate_y::rotate_y(hittable *p, float angle): ptr(p) {
    float radians = (M_PI / 180.0) * angle;
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);
    vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                float x = i * bbox.max().x() + (1 - i) * bbox.min().x();
                float y = j * bbox.max().y() + (1 - j) * bbox.min().y();
                float z = k * bbox.max().z() + (1 - k) * bbox.min().z();
                float newx = cos_theta * x + sin_theta * z;
                float newz = -sin_theta * x + cos_theta * z;
                vec3 tester(newx, y, newz);
                for (int c = 0; c < 3; c++) {
                    if (tester[c] > max[c])
                        max[c] = tester[c];
                    if (tester[c] < min[c])
                        min[c] = tester[c];
                }
            }
        }
    }
    bbox = aabb(min, max);
}

bool rotate_y::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    vec3 origin = r.origin();
    vec3 direction = r.direction();
    origin[0] = cos_theta * r.origin().x() - sin_theta * r.origin().z();
    origin[2] = sin_theta * r.origin().x() + cos_theta * r.origin().z();
    direction[0] = cos_theta * r.direction().x() - sin_theta * r.direction().z();
    direction[2] = sin_theta * r.direction().x() + cos_theta * r.direction().z();
    ray rotated_r(origin, direction, r.time());
    if (ptr->hit(rotated_r, t_min, t_max, rec)) {
        vec3 p = rec.p;
        vec3 normal = rec.normal;
        p[0] = cos_theta * rec.p.x() + sin_theta * rec.p.z();
        p[2] = -sin_theta * rec.p.x() + cos_theta * rec.p.z();
        normal[0] = cos_theta * rec.normal.x() + sin_theta * rec.normal.z();
        normal[2] = -sin_theta * rec.normal.x() + cos_theta * rec.normal.z();
        rec.p = p;
        rec.normal = normal;
        return true;
    }
    else
        return false;
}

#endif
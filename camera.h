#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"
#include "random.h"

class camera {
public:
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, 
        float vfov, // vfov is top to bottom in degree
        float aspect, float aperture, float focus_dist, 
        float t0, float t1) {  

        time0 = t0;
        time1 = t1;
        lens_radius = aperture / 2;
        float theta = vfov * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = half_height * aspect;

        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
        horizontal = 2 * half_width * focus_dist * u;
        vertical = 2 * half_height * focus_dist * v;
    }

    ray get_ray(float s, float t) {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        float time = time0 + random_double() * (time1 - time0);
        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, time);
    }
    
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical; 
    vec3 u, v, w;
    float time0, time1;
    float lens_radius;
};

#endif
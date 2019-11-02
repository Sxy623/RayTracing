#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "random.h"
#include <iostream>
#include <cfloat>
using namespace std;

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(random_double(), random_double(), random_double()) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);
    return p;
}

vec3 color(const ray &r, hittable *world) {
    hit_record rec;
    if (world->hit(r, 0.0, FLT_MAX, rec)) {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5 * color(ray(rec.p, target - rec.p), world);
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        // linear interpolation
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main() {
    int nx = 200;
    int ny = 100;
    int ns = 100;

    freopen("myImage.ppm", "w", stdout);
    
    cout << "P3" << endl;  // The P3 means colors are in ASCII.
    cout << nx << " " << ny << endl;  // numbers of columns and rows
    cout << "255" << endl;  // max color

    hittable *list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hittable *world = new hittable_list(list, 2);

    camera cam;

    for (int j = ny - 1; j >= 0; j--)
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(i + random_double()) / float(nx);
                float v = float(j + random_double()) / float(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world);
            }
            col /= ns;
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            cout << ir << " " << ig << " " << ib << endl;
        }

    fclose(stdout);

    return 0;
}
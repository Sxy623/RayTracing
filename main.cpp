// #define PREVIEW
// #define DEBUG
#define STB_IMAGE_IMPLEMENTATION

#include "camera.h"
#include "random.h"
#include "hittable.h"
#include "scene.h"

#include <iostream>
#include <fstream>
#include <cfloat>
#include <cstdlib>
#include <ctime>
using namespace std;

vec3 color(const ray &r, hittable *world, int depth) {
#ifdef DEBUG
    cout << "color()" << endl;
#endif
    hit_record rec;
    if (world->hit(r, 0.001, FLT_MAX, rec)) {
        ray scattered;
        vec3 attenuation;
        vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return emitted + attenuation * color(scattered, world, depth + 1);
        }
        else {
            return emitted;
        }
    }
    else {
        // vec3 unit_direction = unit_vector(r.direction());
        // float t = 0.5 * (unit_direction.y() + 1.0);
        // return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
        return vec3(0, 0, 0);
    }
}

int main() {
    clock_t start_time, end_time;
    start_time = clock();

    srand(time(NULL));

    int nx, ny, ns;

#ifdef PREVIEW
    nx = 200;
    ny = 200;
    ns = 100;
#else
    nx = 500;
    ny = 500;
    ns = 3000;
#endif

    ofstream image("myImage.ppm");
    
    image << "P3" << endl;  // The P3 means colors are in ASCII.
    image << nx << " " << ny << endl;  // numbers of columns and rows
    image << "255" << endl;  // max color

    // hittable *list[5];
    // list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
    // list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    // list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
    // list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
    // list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
    // hittable *world = new hittable_list(list, 5);

    // hittable *world = random_scene();

    // hittable *world = two_spheres();

    // hittable *world = two_perlin_spheres();

    // hittable *world = image_spheres();

    // hittable *world = simple_light();

    // hittable *world = cornell_box();

    // hittable *world = cornell_smoke();

    hittable *world = final();

    // vec3 lookfrom(278, 278, -800);
    vec3 lookfrom(578, 278, -700);
    vec3 lookat(278, 278, 0);
    float dist_to_focus = 10.0;
    float aperture = 0;
    float vfov = 40;

    camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

    for (int j = ny - 1; j >= 0; j--)
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(i + random_double()) / float(nx);
                float v = float(j + random_double()) / float(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }
            col /= ns;
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            if (col[0] > 1.0) col[0] = 1.0;
            if (col[1] > 1.0) col[1] = 1.0;
            if (col[2] > 1.0) col[2] = 1.0;

            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            image << ir << " " << ig << " " << ib << endl;
        }

    image.close();

    end_time = clock();
    int second = (int)((double)(end_time - start_time) / CLOCKS_PER_SEC);
    int minite = second / 60;
    second = second % 60;
    int hour = minite / 60;
    minite = minite % 60;
    cout << "The run time is: " << hour << " : " << minite << " : " <<  second << endl;

    return 0;
}
#ifndef SCENE_H
#define SCENE_H

#include "stb/stb_image.h"
#include "random.h"

// Material
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "diffuse_light.h"

// Hittable
#include "sphere.h"
#include "moving_sphere.h"
#include "hittable_list.h"
#include "bvh_node.h"
#include "rectangle.h"
#include "flip_normals.h"
#include "box.h"
#include "translate.h"
#include "rotate.h"
#include "constant_medium.h"

// Texture
#include "texture.h"

hittable* random_scene() {
    texture *checker = new checker_texture(
        new constant_texture(vec3(0.2, 0.3, 0.1)),
        new constant_texture(vec3(0.9, 0.9, 0.9))
    );
    int n = 500;
    hittable **list = new hittable*[n + 1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = random_double();
            vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {  // diffuse
                    list[i++] = new moving_sphere(
                        center, center + vec3(0, 0.5 * random_double(), 0),
                        0.0, 1.0, 0.2,
                        new lambertian(new constant_texture(vec3(
                            random_double() * random_double(), 
                            random_double() * random_double(), 
                            random_double() * random_double()
                        )))
                    );
                }
                else if (choose_mat < 0.95) {  // metal
                    list[i++] = new sphere(center, 0.2,
                        new metal(vec3(
                                0.5 * (1 + random_double()), 
                                0.5 * (1 + random_double()), 
                                0.5 * (1 + random_double())
                            ),
                            0.5 * random_double()
                        )
                    );
                } 
                else {  // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    //return new hittable_list(list, i);
    return new bvh_node(list, i, 0.0, 1.0);
}

hittable* two_spheres() {
    texture *checker = new checker_texture(
        new constant_texture(vec3(0.2, 0.3, 0.1)),
        new constant_texture(vec3(0.9, 0.9, 0.9))
    );
    int n = 50;
    hittable **list = new hittable*[n + 1];
    list[0] = new sphere(vec3(0, -10, 0), 10, new lambertian(checker));
    list[1] = new sphere(vec3(0, 10, 0), 10, new lambertian(checker));
    return new hittable_list(list, 2);
}

hittable* two_perlin_spheres() {
    texture *marble = new noise_texture(4.0);
    hittable **list = new hittable*[2];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(marble));
    list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(marble));
    return new hittable_list(list, 2);
}

hittable* image_spheres() {
    texture *marble = new noise_texture(4.0);
    int nx, ny, nn;
    unsigned char *tex_data = stbi_load("resources/earth.jpg", &nx, &ny, &nn, 0);
    hittable **list = new hittable*[2];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(marble));
    list[1] = new sphere(vec3(0, 1, 0), 1, new lambertian(new image_texture(tex_data, nx, ny)));
    return new hittable_list(list, 2);
}

hittable* simple_light() {
    texture *marble = new noise_texture(4.0);
    hittable **list = new hittable*[4];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(marble));
    list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(marble));
    list[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
    list[3] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
    return new hittable_list(list, 4);
}

hittable* cornell_box() {
    hittable **list = new hittable*[8];
    int i = 0;
    material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
    material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
    material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
    material *light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));

    list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
    list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
    list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
    list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
    list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
    list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));

    list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
    list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));

    return new hittable_list(list, i);
}

hittable* cornell_smoke() {
    hittable **list = new hittable*[8];
    int i = 0;
    material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
    material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
    material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
    material *light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));

    list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
    list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
    list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
    list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
    list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
    list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));

    hittable *b1 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
    hittable *b2 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));

    list[i++] = new constant_medium(b1, 0.01, new constant_texture(vec3(1.0, 1.0, 1.0)));
    list[i++] = new constant_medium(b2, 0.01, new constant_texture(vec3(0.0, 0.0, 0.0)));

    return new hittable_list(list, i);
}

hittable* final() {
    int nb = 20;
    hittable **list = new hittable*[30];
    hittable **boxlist = new hittable*[10000];
    hittable **boxlist2 = new hittable*[10000];
    material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
    material *ground = new lambertian(new constant_texture(vec3(0.48, 0.83, 0.53)));

    int b = 0;
    for (int i = 0; i < nb; i++) {
        for (int j = 0; j < nb; j++) {
            float w = 100;
            float x0 = -1000 + i * w;
            float y0 = 0;
            float z0 = -1000 + j * w;
            float x1 = x0 + w;
            float y1 = 100 * (random_double() + 0.01);
            float z1 = z0 + w;
            boxlist[b++] = new box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
        }
    }

    int l = 0;
    list[l++] = new bvh_node(boxlist, b, 0, 1);
    material *light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
    list[l++] = new xz_rect(113, 443, 127, 432, 554, light);

    vec3 center(400, 400, 200);
    list[l++] = new moving_sphere(center, center + vec3(30, 0, 0), 0, 1, 50, new lambertian(new constant_texture(vec3(0.7, 0.3, 0.1))));

    list[l++] = new sphere(vec3(260, 150, 45), 50, new dielectric(1.5));
    list[l++] = new sphere(vec3(0, 150, 145), 50, new metal(vec3(0.8, 0.8, 0.9), 10.0));

    hittable *boundary = new sphere(vec3(360, 150, 145), 70, new dielectric(1.5));
    list[l++] = boundary;
    list[l++] = new constant_medium(boundary, 0.2, new constant_texture(vec3(0.2, 0.4, 0.9)));
    boundary = new sphere(vec3(0, 0, 0), 5000, new dielectric(1.5));
    list[l++] = new constant_medium(boundary, 0.0001, new constant_texture(vec3(1.0, 1.0, 1.0)));

    int nx, ny, nn;
    unsigned char *tex_data = stbi_load("resources/earth.jpg", &nx, &ny, &nn, 0);
    list[l++] = new sphere(vec3(400, 200, 400), 100, new lambertian(new image_texture(tex_data, nx, ny)));
    list[l++] = new sphere(vec3(220, 280, 300), 80, new lambertian(new noise_texture(0.1)));

    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxlist2[j] = new sphere(vec3(165 * random_double(), 165 * random_double(), 165 * random_double()), 10, white);
    }
    list[l++] = new translate(new rotate_y(new bvh_node(boxlist2, ns, 0.0, 1.0), 15), vec3(-100, 270, 395));
    return new hittable_list(list, l);
}

#endif
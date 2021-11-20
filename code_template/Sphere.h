//
// Created by Umur Görkem Karaduman on 2021-11-18.
//

#ifndef GRAPH_HW1_SPHERE_H
#define GRAPH_HW1_SPHERE_H

#include "vec3.h"
#include "parser.h"
#include "utility.h"


class Sphere {
public:
    float radius;
    Vec3<float> center;
    Material material;
    Sphere(float radius, parser::Material, parser::Vec3f center);
};


#endif //GRAPH_HW1_SPHERE_H

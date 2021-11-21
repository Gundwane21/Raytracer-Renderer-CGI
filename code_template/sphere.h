//
// Created by Umur Görkem Karaduman on 2021-11-18.
//

#ifndef GRAPH_HW1_SPHERE_H
#define GRAPH_HW1_SPHERE_H

#include "vec3.h"
#include "parser.h"
#include "utility.h"
#include "shape.h"


class Sphere: public Shape {
public:
    float radius;
    Vec3<float> center;
    Sphere(float radius, parser::Material, parser::Vec3f center);
    Vec3<float> calculateNormalVector(Vec3<float>& intersection_point) override;
};


#endif //GRAPH_HW1_SPHERE_H

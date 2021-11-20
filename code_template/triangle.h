//
// Created by Umur Görkem Karaduman on 2021-11-19.
//

#ifndef GRAPH_HW1_TRIANGLE_H
#define GRAPH_HW1_TRIANGLE_H


#include "parser.h"
#include "utility.h"
#include "vec3.h"

class Triangle {
public:
    Material material;
    Vec3<float> coords[3];
    Triangle(parser::Material, parser::Vec3f , parser::Vec3f, parser::Vec3f);
};


#endif //GRAPH_HW1_TRIANGLE_H

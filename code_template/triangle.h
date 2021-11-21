//
// Created by Umur Görkem Karaduman on 2021-11-19.
//

#ifndef GRAPH_HW1_TRIANGLE_H
#define GRAPH_HW1_TRIANGLE_H


#include "parser.h"
#include "utility.h"
#include "vec3.h"
#include "shape.h"

class Triangle: public Shape{
public:
    Vec3<float> coords[3];
    Vec3<float> normal;
    Triangle(parser::Material, parser::Vec3f , parser::Vec3f, parser::Vec3f);
    Vec3<float> calculateNormalVector(Vec3<float>& intersection_point) override;
};


#endif //GRAPH_HW1_TRIANGLE_H

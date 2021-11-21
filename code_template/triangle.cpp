//
// Created by Umur Görkem Karaduman on 2021-11-19.
//

#include "triangle.h"


Triangle::Triangle(parser::Material material, parser::Vec3f v0, parser::Vec3f v1, parser::Vec3f v2) : Shape(material){
    this->coords[0] = v0;
    this->coords[1] = v1;
    this->coords[2] = v2;
    Vec3<float> vec1 = coords[0].subtVector(coords[1]);
    Vec3<float> vec2 = coords[2].subtVector(coords[1]);
    this->normal = vec2.cross(vec1).normalize();
}

Vec3<float> Triangle::calculateNormalVector(Vec3<float>& intersection_point) {
    return this->normal;
}
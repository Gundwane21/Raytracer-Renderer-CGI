//
// Created by Umur Görkem Karaduman on 2021-11-19.
//

#include "triangle.h"


Triangle::Triangle(parser::Material material, parser::Vec3f v0, parser::Vec3f v1, parser::Vec3f v2) : material(Material(material)){
    this->coords[0] = v0;
    this->coords[1] = v1;
    this->coords[2] = v2;
}
//
// Created by Umur Görkem Karaduman on 2021-11-18.
//

#include "sphere.h"

Sphere::Sphere(float radius, parser::Material material, parser::Vec3f center): radius(radius), Shape(material), center(Vec3<float>(center)){
}

Vec3<float> Sphere::calculateNormalVector(Vec3<float>& intersection_point) {
    return intersection_point.subtVector(this->center);
}


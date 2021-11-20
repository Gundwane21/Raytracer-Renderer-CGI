//
// Created by Umur Görkem Karaduman on 2021-11-18.
//

#include "sphere.h"

Sphere::Sphere(float radius, parser::Material material, parser::Vec3f center): radius(radius),
material(Material(material)),
center(Vec3<float>(center)){
}

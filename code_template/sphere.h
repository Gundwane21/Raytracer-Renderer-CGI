//
// Created by Umur Görkem Karaduman on 2021-11-18.
//

#ifndef GRAPH_HW1_SPHERE_H
#define GRAPH_HW1_SPHERE_H

#include "vec3.h"


class Sphere {
public:
    int sphere_id;
    int material_id;
    int center_vertex_id;
    float radius;
    Sphere();
    Sphere(int sphere_id ,const parser::Sphere & sphere);
    
};


#endif //GRAPH_HW1_SPHERE_H

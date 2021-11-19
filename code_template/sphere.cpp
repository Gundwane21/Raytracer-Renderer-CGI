//
// Created by Umur Görkem Karaduman on 2021-11-18.
//

#include "sphere.h"

Sphere::Sphere(){    
    this->center_vertex_id = 0 ;
    this->center_vertex_id =0;
    this->radius = 0 ;
    this->sphere_id = 0;
}

Sphere::Sphere(int sphere_id ,const parser::Sphere & sphere){
    this->material_id = sphere.material_id;
    this->center_vertex_id = sphere.center_vertex_id;
    this->radius = sphere.radius;
    this->sphere_id = sphere_id;
}
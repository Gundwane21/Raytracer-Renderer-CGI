//
// Created by Umur Görkem Karaduman on 2021-11-21.
//

#ifndef GRAPH_HW1_MESH_H
#define GRAPH_HW1_MESH_H

#include "vec3.h"
#include "utility.h"
#include "triangle.h"

class Mesh {
public:
    Material material;
    std::vector<Triangle> mesh_faces;
    Mesh(parser::Material , std::vector<Triangle> &);
};


#endif //GRAPH_HW1_MESH_H

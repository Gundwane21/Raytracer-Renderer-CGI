//
// Created by Umur Görkem Karaduman on 2021-11-21.
//

#include "mesh.h"

#include <utility>


Mesh::Mesh(parser::Material material, std::vector<Triangle>& triangle): material(material){
    this->mesh_faces = std::move(triangle);
}
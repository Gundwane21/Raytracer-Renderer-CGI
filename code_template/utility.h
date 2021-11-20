//
// Created by Umur Görkem Karaduman on 2021-11-18.
//

#ifndef GRAPH_HW1_UTILITY_H
#define GRAPH_HW1_UTILITY_H


struct ImagePlane{
    float left, right, bottom, top;
    float distance;
};


struct Material
{
    bool is_mirror;
    Vec3<float> ambient;
    Vec3<float> diffuse;
    Vec3<float> specular;
    Vec3<float> mirror;
    float phong_exponent;
    explicit Material(parser::Material material){
        this->is_mirror = material.is_mirror;
        this->ambient = Vec3<float>(material.ambient);
        this->diffuse = Vec3<float>(material.diffuse);
        this->specular = Vec3<float>(material.specular);
        this->mirror = Vec3<float>(material.mirror);
        this->phong_exponent = material.phong_exponent;
    }
};

typedef struct {
    int id;
    Vec3<float> position;
    Vec3<float> intensity;
}PointLightSource;


#endif //GRAPH_HW1_UTILITY_H

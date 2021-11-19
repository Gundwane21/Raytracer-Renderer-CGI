//
// Created by Umur Görkem Karaduman on 2021-11-18.
//

#ifndef GRAPH_HW1_UTILITY_H
#define GRAPH_HW1_UTILITY_H


struct ImagePlane{
    float left, right, bottom, top;
    float distance;
};

typedef struct {
    int id;
    Vec3<float> ambient_reflectance;
    Vec3<float> diffuse_reflectance;
    Vec3<float> specular_reflectance;
    Vec3<float> mirror_reflectance; // zero means no mirror
    bool is_mirror;
    float phong_exponent; // exponent used in bling phong shading
    
}Material;

typedef struct {
    int id;
    Vec3<float> position;
    Vec3<float> intensity;
}PointLightSource;


#endif //GRAPH_HW1_UTILITY_H

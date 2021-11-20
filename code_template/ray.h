//
//

#ifndef GRAPH_HW1_RAY_H
#define GRAPH_HW1_RAY_H

#include "vec3.h"
#include "sphere.h"
#include "utility.h"
#include "camera_bundle.h"
#include "sphere.h"
#include "triangle.h"
#include <cmath>
class Ray {
public:
    Vec3<float> o;
    Vec3<float> d;
    Ray(int i,int j, float pixel_width, float pixel_height, const CameraBundle& camera_bundle);
    float intersectRayWithSphere(Sphere sphere);
    float intersectRayWithTriangle(Triangle triangle);
    Vec3<float> computeColor(std::vector<Sphere> sphere_list, std::vector<Triangle> triangle_list,  Vec3<float> light);
    static float calculateDeterminant(const std::vector<std::vector<float>> & matrix);
};


#endif //GRAPH_HW1_RAY_H

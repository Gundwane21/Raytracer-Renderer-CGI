//
//

#ifndef GRAPH_HW1_RAY_H
#define GRAPH_HW1_RAY_H

#include "vec3.h"
#include "sphere.h"
#include "utility.h"
#include "camera_bundle.h"
#include "sphere.h"
#include "math.h"
class Ray {
public:
    Vec3<float> o;
    Vec3<float> d;
    Ray(int i,int j, float pixel_width, float pixel_height, const CameraBundle& camera_bundle);

    float intersectRayWithSphere(Sphere sphere);

    //Vec3<float> computeColor(Sphere sphereList[], int sphereNum,  Vec3<float> light);
};


#endif //GRAPH_HW1_RAY_H

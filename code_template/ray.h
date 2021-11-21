//
//

#ifndef GRAPH_HW1_RAY_H
#define GRAPH_HW1_RAY_H

#include "vec3.h"
#include "sphere.h"
#include "utility.h"
#include "camera_bundle.h"
#include "sphere.h"
#include "parser.h"

#include <vector> // ask TA's if we can use it!
#include <limits>

#include "triangle.h"
#include <cmath>
#include "mesh.h"
class Ray {
public:
    Vec3<float> o;
    Vec3<float> d;
    Ray(int i,int j, float pixel_width, float pixel_height, const CameraBundle& camera_bundle);
    float intersectRayWithTriangle(Triangle triangle);
    static float calculateDeterminant(const std::vector<std::vector<float>> & matrix);
    Ray(Vec3<float> origin,Vec3<float> direction);
    float intersectRayWithSphere(Sphere sphere);

    Vec3<float> computeColor( Vec3<int> background_color,float shadow_ray_epsilon,
                              Vec3<float> ambient_light , std::vector<PointLightSource> & point_lights, std::vector<Sphere> & spheres
                              , std::vector<Triangle> & triangles);
    Vec3<float> calculateNormalVec(Vec3<float> intersection_point,Vec3<float> sphere_center_vertex);
};


#endif //GRAPH_HW1_RAY_H

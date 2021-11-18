#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "ray.h"
#include "utility.h"
#include "camera_bundle.h"
#include "vec3.h"


int main(int argc, char * argv[]){
    Vec3<float> eye(0,0,0);
    Vec3<float> gaze(0, 0, -1);
    Vec3<float> u(1,0,0);
    Vec3<float> v(0,1, 0);
    struct ImagePlane im_plane;
    im_plane.left = -1;
    im_plane.right = 1;
    im_plane.top = 1;
    im_plane.bottom = -1;
    im_plane.distance = 1;
    CameraBundle cb = {eye, gaze, u, v, im_plane};
    float pixel_width = (im_plane.right-im_plane.left)/1024;
    float pixel_height = (im_plane.top-im_plane.bottom)/768;
    Ray ray(256, 192, pixel_width, pixel_height, cb);
    std::cout<<"RAY.X: " << ray.o.x << " RAY.Y: " << ray.o.y <<" RAY.Z " << ray.o.z << std::endl;
    std::cout<<"RAY.X: " << ray.d.x << " RAY.Y: " << ray.d.y <<" RAY.Z " << ray.d.z << std::endl;
    return 0;
}
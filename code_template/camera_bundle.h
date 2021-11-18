//
// Created by Umur Görkem Karaduman on 2021-11-18.
//

#ifndef GRAPH_HW1_CAMERABUNDLE_H
#define GRAPH_HW1_CAMERABUNDLE_H

#include "vec3.h"
#include "utility.h"


struct CameraBundle{
    const Vec3<float> & eye;
    const Vec3<float> & gaze;
    const Vec3<float> & u;
    const Vec3<float> & v;
    const ImagePlane & im_plane;
};


#endif //GRAPH_HW1_CAMERABUNDLE_H

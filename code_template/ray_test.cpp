//
//

#include <gtest/gtest.h>
#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "ray.h"
#include "utility.h"
#include "camera_bundle.h"
#include "vec3.h"
#include "triangle.h"


TEST(RayTester, RayGeometrySlide15) {
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
    EXPECT_NEAR(ray.o.x, 0, 0.0001);
    EXPECT_NEAR(ray.o.y, 0, 0.0001);
    EXPECT_NEAR(ray.o.z, 0, 0.0001);
    EXPECT_NEAR(ray.d.x, -0.49902, 0.0001);
    EXPECT_NEAR(ray.d.y, 0.5 - 1.0/768, 0.0001);
    EXPECT_NEAR(ray.d.z, -1, 0.0001);
}

TEST(RayTriangleIntersection, DeterminantCheck){
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
    parser::Vec3f a{2, 2, 1};
    parser::Vec3f b{-3, 0, 4};
    parser::Vec3f c{1, -1, 5};
    std::vector<std::vector<float>> m{{2,2,1},{-3,0,4},{1,-1,5}};
    parser::Material material{true, {1,1,1}, {1,1,1},{0,0,1},{2,2,2},43.5};
    Triangle triangle(material, a, b,c);
    EXPECT_NEAR(ray.calculateDeterminant(m), 49, 0.0001);
}




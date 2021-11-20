#include <iostream>
#include "parser.h"
#include "ppm.h"
#include "ray.h"
#include "utility.h"
#include "camera_bundle.h"
#include "vec3.h"
#include "triangle.h"

typedef unsigned char RGB[3];

int main(int argc, char* argv[])
{
    // Sample usage for reading an XML scene file
    parser::Scene scene;

    scene.loadFromXml(argv[1]);

    // The code below creates a test pattern and writes
    // it to a PPM file to demonstrate the usage of the
    // ppm_write function.
    //
    // Normally, you would be running your ray tracing
    // code here to produce the desired image.

    const RGB BAR_COLOR[8] =
    {
        { 255, 255, 255 },  // 100% White
        { 255, 255,   0 },  // Yellow
        {   0, 255, 255 },  // Cyan
        {   0, 255,   0 },  // Green
        { 255,   0, 255 },  // Magenta
        { 255,   0,   0 },  // Red
        {   0,   0, 255 },  // Blue
        {   0,   0,   0 },  // Black
    };

    std::vector<Sphere> spheres;
    std::vector<Triangle> triangles;
    for(int i = 0; i<scene.spheres.size(); i++){
        parser::Sphere sphere = scene.spheres[i];
        spheres.emplace_back(sphere.radius, scene.materials[sphere.material_id], scene.vertex_data[sphere.center_vertex_id]);
    }
    for(int i = 0; i<scene.triangles.size(); i++){
        parser::Triangle triangle = scene.triangles[i];
        triangles.emplace_back(scene.materials[triangle.material_id], scene.vertex_data[triangle.indices.v0_id], scene.vertex_data[triangle.indices.v1_id], scene.vertex_data[triangle.indices.v2_id]);
    }

    for(auto & cam_config : scene.cameras){
        int image_width = cam_config.image_width, image_height = cam_config.image_height;
        struct ImagePlane im_plane = {cam_config.near_plane.x , cam_config.near_plane.y, cam_config.near_plane.z, cam_config.near_plane.w, cam_config.near_distance};
        int columnWidth = image_width/8;
        unsigned char* image = new unsigned char [image_width * image_height * 3];
        float pixel_width = (im_plane.right-im_plane.left)/image_width;
        float pixel_height =  (im_plane.top-im_plane.bottom)/image_height;
        Vec3<float> eye(cam_config.position);
        Vec3<float> gaze(cam_config.gaze);
        Vec3<float> up(cam_config.up);
        CameraBundle camera_bundle={eye, gaze, up.cross(gaze.multScalar(-1.0)), cam_config.up,im_plane};

        /* RAY TRACER LOOP CAUTION */
        //TODO: implement the loop
       /*
        for each pixel do
            compute viewing (eye, primary) rays
            find the first object hit by ray and its surface normal n
            set pixel color to value computed from hit point, light, and n
        */

        for(int j = 0; j < image_height; j++){
            for(int i = 0; i < image_width; i++){
                Ray ray(i,j, pixel_width, pixel_height, camera_bundle);
                Vec3<float> pixel;
                Vec3<float> rayColor;
                pixel = ray.o.addVector(ray.d);
                rayColor = ray.computeColor(spheres, light);
//                image[i][j].x = (int) (rayColor.x * 255+0.5);
//                image[i][j].y = (int) (rayColor.y * 255+0.5);
//                image[i][j].z = (int) (rayColor.z * 255+0.5);
            }
        }
        write_ppm(cam_config.image_name.c_str(), image, image_width, image_height);
    }

}

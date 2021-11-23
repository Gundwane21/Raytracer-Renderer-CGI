#include <iostream>
#include <thread>

#include "parser.h"
#include "ppm.h"
#include "ray.h"
#include "utility.h"
#include "camera_bundle.h"
#include "vec3.h"
#include "mesh.h"

int bunny_im_height = 512;

/* global variables */
Vec3<int> background_color ;
Vec3<float> ambient_light;
float shadow_ray_epsilon ;
typedef unsigned char RGB[3];


//void writePPM(Vec3<int> **image ,int nx,int ny)
//{
//	int i,j;
//	FILE *fp;
//
//	fp = fopen("output.ppm","w");
//
//	fprintf(fp,"P3\n");
//	fprintf(fp,"#output.ppm\n");
//	fprintf(fp,"%d %d\n",nx,ny);
//	fprintf(fp,"255\n");
//	for (j=0;j<ny;j++)
//	{
//		for (i=0;i<nx;i++)
//		{
//			fprintf(fp,"%d %d %d\t",image[i][j].x,image[i][j].y,image[i][j].z);
//		}
//		fprintf(fp,"\n");
//	}
//	//system("convert output.ppm output.png");
//	//system("rm output.ppm");
//
//}


void computeRaytracerThread(unsigned char * image, int height_start, int height_end , CameraBundle& camera_bundle
                            , int  image_width, int image_height ,std::vector<Sphere> &spheres, std::vector<Triangle> &triangles
                            ,std::vector<PointLightSource> &point_lights, float  pixel_width,float  pixel_height  ){
    for(int j = height_start; j < height_end; j++){
        for(int i = 0; i < image_width*3; i+=3){
            Ray ray(i/3,j, pixel_width, pixel_height, camera_bundle);
            std::cout<< j<< "/" << bunny_im_height << " | " << i << "/" << image_width << "\r";
            Vec3<float> pixel;
            Vec3<float> rayColor;
            pixel = ray.o.addVector(ray.d);
            rayColor = ray.computeColor(background_color,shadow_ray_epsilon,ambient_light,point_lights ,spheres,triangles);
            image[j*image_width*3 + i] = (unsigned char) rayColor.x;
            image[j*image_width*3 + i +1] = (unsigned char) rayColor.y;
            image[j*image_width*3 + i +2] = (unsigned char) rayColor.z;
        }
    }
}


int main(int argc, char* argv[]) {
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
                    {255, 255, 255},  // 100% White
                    {255, 255, 0},  // Yellow
                    {0,   255, 255},  // Cyan
                    {0,   255, 0},  // Green
                    {255, 0,   255},  // Magenta
                    {255, 0,   0},  // Red
                    {0,   0,   255},  // Blue
                    {0,   0,   0},  // Black
            };

    background_color = Vec3<int>(scene.background_color.x, scene.background_color.y, scene.background_color.z);
    ambient_light = Vec3<float>(scene.ambient_light);
    shadow_ray_epsilon = scene.shadow_ray_epsilon;

    /* Get All Point Light Sources*/
    std::vector<PointLightSource> point_lights;
    int point_light_id = 1;
    for (auto &point_lights_config: scene.point_lights) {

        PointLightSource point_light{point_light_id, point_lights_config.position, point_lights_config.intensity};
        point_lights.push_back(point_light);
        point_light_id++;
    }
    /*Get All Objects */
    std::vector<Sphere> spheres;
    std::vector<Triangle> triangles;
    std::vector<Mesh> meshes;
    std::vector<Shape *> shapes;

    /* Get All Spheres*/
    for (auto &sphere: scene.spheres) {
        spheres.emplace_back(sphere.radius, scene.materials[sphere.material_id - 1],
                             scene.vertex_data[sphere.center_vertex_id - 1]);
        shapes.emplace_back(new Sphere(sphere.radius, scene.materials[sphere.material_id - 1],
                                       scene.vertex_data[sphere.center_vertex_id - 1]));
    }
    /* Get All Triangles*/
    for (auto &triangle: scene.triangles) {
        triangles.emplace_back(scene.materials[triangle.material_id - 1], scene.vertex_data[triangle.indices.v0_id - 1],
                               scene.vertex_data[triangle.indices.v1_id - 1],
                               scene.vertex_data[triangle.indices.v2_id - 1]);
    }

    /* Get All Meshes as Triangles*/
    for (auto &mesh: scene.meshes) {
        for (auto &face: mesh.faces) {
            triangles.emplace_back(scene.materials[mesh.material_id - 1], scene.vertex_data[face.v0_id - 1],
                                   scene.vertex_data[face.v1_id - 1], scene.vertex_data[face.v2_id - 1]);
        }
    }

    /* Get All Cameras and run raytracer loop */


    for (auto &cam_config: scene.cameras) {
        int image_width = cam_config.image_width, image_height = cam_config.image_height;
        struct ImagePlane im_plane = {cam_config.near_plane.x, cam_config.near_plane.y, cam_config.near_plane.z,
                                      cam_config.near_plane.w, cam_config.near_distance};
        int columnWidth = image_width / 8;

        unsigned char* image = new unsigned char[image_width*image_height*3]{0, 0, 0};



        float pixel_width = (im_plane.right - im_plane.left) / image_width;
        float pixel_height = (im_plane.top - im_plane.bottom) / image_height;
        Vec3<float> eye(cam_config.position);
        Vec3<float> gaze(cam_config.gaze);
        Vec3<float> up(cam_config.up);
        CameraBundle camera_bundle = {eye, gaze, up.cross(gaze.multScalar(-1.0)), cam_config.up, im_plane};

        /* RAY TRACER LOOP CAUTION */
        //TODO: implement the loop
        /*
         for each pixel do
             compute viewing (eye, primary) rays
             find the first object hit by ray and its surface normal n
             set pixel color to value computed from hit point, light, and n
         */

        unsigned int thread_limit = std::thread::hardware_concurrency();
        std::vector<std::thread*> threads;
        Ray ray(400,400,pixel_width, pixel_height, camera_bundle);
        ray.computeColor(background_color,shadow_ray_epsilon,ambient_light,point_lights ,spheres,triangles);
        int ratio = image_height / thread_limit;
        for (unsigned int i = 0; i < thread_limit; i++) {

            int height_start = ratio * i;
            int height_end = ratio * (i + 1);

            if (i == thread_limit - 1) {
                height_end = image_height;
            }
            printf("start:%d, end: %d\n", height_start, height_end);
            for(int k = 0 ; k < i ; k++)
                printf("\n");
            std::thread* thread = new std::thread (computeRaytracerThread, image, height_start, height_end, std::ref(camera_bundle),
                               image_width, image_height, std::ref(spheres), std::ref(triangles),
                               std::ref(point_lights),
                               pixel_width, pixel_height);
            threads.push_back(thread);


        }
        for (auto & thread: threads){
            thread->join();
            delete thread;
        }
        write_ppm(cam_config.image_name.c_str(),  image, image_width, image_height);
    }
    return 0;
}

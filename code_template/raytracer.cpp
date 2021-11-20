#include <iostream>
#include <vector> // ask TA's if we can use it!

#include "parser.h"
#include "ppm.h"
#include "ray.h"
#include "utility.h"
#include "camera_bundle.h"
#include "vec3.h"

typedef unsigned char RGB[3];

void writePPM(Vec3<int> **image ,int nx,int ny)
{
	int i,j;
	FILE *fp;
	
	fp = fopen("output.ppm","w");
	
	fprintf(fp,"P3\n");
	fprintf(fp,"#output.ppm\n");
	fprintf(fp,"%d %d\n",nx,ny);
	fprintf(fp,"255\n");
	for (j=0;j<ny;j++)
	{
		for (i=0;i<nx;i++)
		{
			fprintf(fp,"%d %d %d\t",image[i][j].x,image[i][j].y,image[i][j].z);
		}
		fprintf(fp,"\n");
	}
	//system("convert output.ppm output.png");
	//system("rm output.ppm");
	
}


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

    Vec3<int> background_color(scene.background_color.x,scene.background_color.y,scene.background_color.z) ;
    Vec3<float> ambient_light(scene.ambient_light);
    float shadow_ray_epsilon = scene.shadow_ray_epsilon;
 
    /* Get All Materials*/
    std::vector<Material> materials;
    int material_id = 1; //look for id in parser!! make sure,  does it always go as 1,2...         
    for( auto & material_config: scene.materials){
        
        Material material = { material_id , material_config.ambient , material_config.diffuse, material_config.specular
        , material_config.mirror, material_config.is_mirror , material_config.phong_exponent }; 
        materials.push_back(material);
        material_id++;
    }
    /* Get All Point Light Sources*/
    std::vector<PointLightSource> point_lights;
    int point_light_id = 1;
    for(auto & point_lights_config : scene.point_lights){

        PointLightSource point_light{point_light_id, point_lights_config.position , point_lights_config.intensity};
        point_lights.push_back(point_light);
        point_light_id++;
    }
    /* Get All Spheres*/
    std::vector<Sphere> spheres;
    int sphere_id = 1;
    for(auto & sphere_config : scene.spheres){
        
        Sphere sphere(sphere_id,sphere_config);
        spheres.push_back(sphere);
        sphere_id++;
    }
    /* Get All Vertices*/
    std::vector<Vec3<float>> vertices;
    int vertex_id = 1;
    for(auto & vertex_config : scene.vertex_data){
        
        vertices.push_back(vertex_config);
        vertex_id++;
    }

    /* Get All Cameras and run raytracer loop */
    for(auto & cam_config : scene.cameras){
        int image_width = cam_config.image_width, image_height = cam_config.image_height;
        struct ImagePlane im_plane = {cam_config.near_plane.x , cam_config.near_plane.y, cam_config.near_plane.z, cam_config.near_plane.w, cam_config.near_distance};
        int columnWidth = image_width/8;
        
        //unsigned char* image = new unsigned char [image_width * image_height * 3];
        /*  */
        Vec3<int> ** image;
        image = new Vec3<int>*[image_width];

        for(int i =0 ; i < image_width  ;i++){
            image[i] = new   Vec3<int>[image_height];  
        }

        for (int  j  = 0 ; j <   image_height ; j++){
            for(int i =0  ; i <  image_width ;i++ ){    
                image[i][j].x  =  image[i][j].y = image[i][j].z = 0; 
            }
        }   

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
                rayColor = ray.computeColor(spheres,background_color,shadow_ray_epsilon,ambient_light,materials,point_lights,vertices);
                // if (!(rayColor.x == 0 && rayColor.y == 0 && rayColor.z == 0 )) 
                //     printf("raycolor x : %f , y : %f , z: %f \n", rayColor.x, rayColor.y , rayColor.z);

                image[i][j].x = (int) rayColor.x;
                image[i][j].y = (int) rayColor.y;
                image[i][j].z = (int) rayColor.z;            
                // image[i * image_width +  j * image_height] = rayColor.x;
                // image[i * image_width +  j * image_height + 1] = rayColor.y;
                // image[i * image_width +  j * image_height + 2] = rayColor.z;
                
            }
        }
        //write_ppm(cam_config.image_name.c_str(), image, image_width, image_height);
        writePPM(image, image_width, image_height);
    }

}

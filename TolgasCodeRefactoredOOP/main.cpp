#include "raytracer.hpp"

int main(int argc, char * argv[]){
    
    //hardcode light
    Vec3f light(0,50,0);

    imagePlane implane;
    scanf("%d %d",&implane.nx,&implane.ny);
	scanf("%f %f %f %f %f",&implane.dist,&implane.left,&implane.right,&implane.bottom,&implane.top);
	
    scanf("%f %f %f",&(eye.x),&(eye.y),&(eye.z));
	scanf("%f %f %f",&(gaze.x),&(gaze.y),&(gaze.z));
	scanf("%f %f %f",&(v.x),&(v.y),&(v.z));

    int sphereNum= 2;
    Sphere sphereArr[sphereNum];

    w = gaze.multScaler(-1);
	u.x = 1.0;
	u.y = u.z = 0.0;

    Vec3i ** image;
    image = new Vec3i*[implane.nx];

    for(int i =0 ; i < implane.nx  ;i++){
        image[i] = new   Vec3i[implane.ny];  
    }

    for (int  j  = 0 ; j <   implane.ny ; j++){
        for(int i =0  ; i <  implane.nx ;i++ ){    
            image[i][j].x  =  image[i][j].y = image[i][j].x=0; 
        }
    }   
    
    float  pixelWidth= (implane.right - implane.left)/implane.nx ;
    float  pixelHeight= (implane.top - implane.bottom)/implane.ny ;
                    
    /*RAY  TRACE LOOP */
    for (int j=0; j<implane.ny ;j++){
        for (int i=0;i<implane.nx;i++){

			Ray ray(i,j, pixelWidth, pixelHeight,implane);
            Vec3f pixel;
            Vec3f rayColor;
			pixel = ray.o.addVector(ray.d);
			rayColor = ray.computeColor(sphereArr,sphereNum,light);
            image[i][j].x = (int) (rayColor.x * 255+0.5);
            image[i][j].y = (int) (rayColor.y * 255+0.5);
            image[i][j].z = (int) (rayColor.z * 255+0.5);
            //printf("%.4f %.4f %.4f\n",myray.d.x,myray.d.y,myray.d.z);
			//printf("%.4f %.4f %.4f\n",pixel.x,pixel.y,pixel.z);
		}
    }
    writePPM(image,implane.nx,implane.ny);
    for(int i =0 ; i < implane.nx  ;i++){
        delete[] image[i];  
    }
    delete[] image;
    return 0;
}
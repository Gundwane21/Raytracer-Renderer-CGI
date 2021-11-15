#include "raytracer.hpp"

Vec3i::Vec3i(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vec3i::Vec3i(int x, int y, int z){
    this->x = x ;
    this->y = y ; 
    this->z = z ;
}

Vec3i Vec3i::multScaler(float scalar){
    Vec3i result;
    result.x = this->x *scalar;
    result.y = this->y *scalar;
    result.z = this->z * scalar;
    return result;
}

Vec3i Vec3i::addVector(const Vec3i vec2){
    Vec3i result;
    result.x = this->x + vec2.x ;
    result.y = this->y + vec2.y ;
    result.z = this->z + vec2.z ;
    return result;
}

float Vec3i::dot(Vec3i vec2){
    return this->x * vec2.x + this->y * vec2.y + this->z * vec2.z ;
}

Vec3i Vec3i::normalize(){
    return multScaler(1.0/sqrt(this->dot(*this)));
}


Vec3f::Vec3f(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vec3f::Vec3f(float x, float y, float z){
    this->x = x ;
    this->y = y ; 
    this->z = z ;
}

Vec3f Vec3f::multScaler(float scalar){
    Vec3f result;
    result.x = this->x *scalar;
    result.y = this->y *scalar;
    result.z = this->z * scalar;
    return result;
}

Vec3f Vec3f::addVector(const Vec3f vec2){
    Vec3f result;
    result.x = this->x + vec2.x ;
    result.y = this->y + vec2.y ;
    result.z = this->z + vec2.z ;
    return result;
}

float Vec3f::dot(Vec3f vec2){
    return this->x * vec2.x + this->y * vec2.y + this->z * vec2.z ;
}

Vec3f Vec3f::normalize(){
    return multScaler(1.0/sqrt(this->dot(*this)));
}

Vec3f eye,gaze,u,v,w;

Ray::Ray(const int i,const int j, const float pixelWidth, const float pixelHeight,
    const imagePlane implane){
    float su,sv;
    Vec3f m,q,s;

    su = (i + 0.5) *  pixelWidth;
    sv = (j + 0.5) *  pixelHeight;

    m = eye.addVector(gaze.multScaler(implane.dist));
    q = m.addVector( (u.multScaler(implane.left)).addVector(v.multScaler(implane.top)));

    s = q.addVector( (u.multScaler(su)).addVector(v.multScaler(-sv)) );

    this->o  = eye ;
    this->d  = s.addVector(eye.multScaler(-1));

}

Vec3f Ray::computeColor(Sphere sphereList[], int sphereNum,  Vec3f light){
    float  minT = 9000;
    int minI = -1;
    float t;//intersection t 

    Vec3f color;
    Vec3f normal;

    Vec3f intersectionPoint;
    Vec3f pointToLight;

    for (int i = 0 ; i < sphereNum ; i++){
        t = this->intersectRayWithSphere( sphereList[i]);
        //printf("sphere %d t: %f, minT: %f   \n", i , t , minT);
        if (t < minT && t >= 0){ //could be t >= 0
            color = sphereList[i].color;
            minI = i;
            minT = t;
        }
    }


    if(minI != -1) {
        intersectionPoint =  this->o.addVector(this->d.multScaler(minT));
        pointToLight = light.addVector(intersectionPoint.multScaler(-1));
        normal = intersectionPoint.addVector(sphereList[minI].center.multScaler(-1));
        pointToLight = pointToLight.normalize();
        normal = normal.normalize();

        float dotProduct = pointToLight.dot(normal);
        if (dotProduct > 0)
            color = color.multScaler(dotProduct);
        else
            color.x = color.y = color.z = 0;
    } 
    return color;

}


float  Ray::intersectRayWithSphere( Sphere sphere){
    float  A,B,C;
    float delta;

    Vec3f c  = sphere.center;
    float  t,t1,t2;

    B = 2*this->d.x*(this->o.x-c.x)+2*this->d.y*(this->o.y-c.y)+2*this->d.z*(this->o.z-c.z);
    C = (this->o.x-c.x)*(this->o.x-c.x)+(this->o.y-c.y)*(this->o.y-c.y)+(this->o.z-c.z)*(this->o.z-c.z)-sphere.radius*sphere.radius;


    A = this->d.x*this->d.x+this->d.y*this->d.y+this->d.z*this->d.z;

    delta=  B*B- 4*A*C;

    if (delta<0) return -1;
    else if (delta==0)
    {
        t = -B / (2*A);
    }
    else
    {
        delta = sqrt(delta);
        A = 2*A;
        t1 = (-B + delta) / A;
        t2 = (-B - delta) / A;
                
        if (t1<t2) t=t1; else t=t2;
    }       
    return t;
}

Sphere::Sphere(){
    this->radius = 0 ;
    Vec3f  vec1,vec2;
    this->center = vec1 ;
    this->color = vec2;
    scanf("%f %f %f %f %f %f %f",&(center.x),&(center.y),&(center.z),&(radius),&(color.x),&(color.y),&(color.z));	
}


void writePPM(Vec3i** image, int  nx, int ny );

int main(){

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

}

void writePPM(Vec3i** image, int  nx, int ny ){
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
}

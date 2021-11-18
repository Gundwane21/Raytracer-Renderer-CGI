//
//


#include "ray.h"


Ray::Ray(const int i, const int j, const float pixel_width, const float pixel_height, const CameraBundle& cb){
    float s_u, s_v;
    Vec3<float> m, q, s;
    s_u = (i + 0.5) *  pixel_width;
    s_v = (j + 0.5) *  pixel_height;
    m = cb.eye.addVector(cb.gaze.multScalar(cb.im_plane.distance));
    q = m.addVector((cb.u.multScalar(cb.im_plane.left)).addVector(cb.v.multScalar(cb.im_plane.top)));
    s = q.addVector((cb.u.multScalar(s_u)).addVector(cb.v.multScalar(-s_v)) );
    this->o  = cb.eye;
    this->d  = s.addVector(cb.eye.multScalar(-1));
}

//Vec3<float> Ray::computeColor(Sphere sphereList[], int sphereNum,  Vec3<float> light){
//    float  minT = 9000;
//    int minI = -1;
//    float t;//intersection t
//
//    Vec3<float> color;
//    Vec3<float> normal;
//
//    Vec3<float> intersectionPoint;
//    Vec3<float> pointToLight;
//
//    for (int i = 0 ; i < sphereNum ; i++){
//        t = this->intersectRayWithSphere( sphereList[i]);
//        //printf("sphere %d t: %f, minT: %f   \n", i , t , minT);
//        if (t < minT && t >= 0){ //could be t >= 0
//            color = sphereList[i].color;
//            minI = i;
//            minT = t;
//        }
//    }
//
//
//    if(minI != -1) {
//        intersectionPoint =  this->o.addVector(this->d.multScalar(minT));
//        pointToLight = light.addVector(intersectionPoint.multScalar(-1));
//        normal = intersectionPoint.addVector(sphereList[minI].center.multScalar(-1));
//        pointToLight = pointToLight.normalize();
//        normal = normal.normalize();
//
//        float dotProduct = pointToLight.dot(normal);
//        if (dotProduct > 0)
//            color = color.multScalar(dotProduct);
//        else
//            color.x = color.y = color.z = 0;
//    }
//    return color;
//
//}


float Ray::intersectRayWithSphere( Sphere sphere){
    float  A,B,C;
    float delta;
    Vec3<float> c  = sphere.center;
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

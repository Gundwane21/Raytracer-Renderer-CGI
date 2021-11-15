#include <bits/stdc++.h>
#include <limits>

using namespace std;

typedef struct {
    int  nx,  ny ;
    float  dist,left,right,top,bottom;
} imagePlane ;

class Vec3i
{
    public:
    int x,y,z;

    public:
    Vec3i();
    Vec3i(int x, int y, int z);
    
    Vec3i multScaler(float scalar);
    Vec3i addVector(const Vec3i vec2);

    float dot(Vec3i vec2);
    Vec3i normalize();

};


class Vec3f
{
    public:
    float x,y,z;

    public:
    Vec3f();
    Vec3f(float x, float y, float z);
    
    Vec3f multScaler(float scalar);
    Vec3f addVector(const Vec3f vec2);

    float dot(Vec3f vec2);
    Vec3f normalize();

};


class Sphere {
    public:
    Vec3f center,color;
    float radius;
    
    Sphere();
    
};

class Ray {
    public:
    Vec3f o;
    Vec3f d;
    Ray(const int i,const int j, const float pixelWidth, const float pixelHeight,
        const imagePlane implane);

    float  intersectRayWithSphere(Sphere sphere);

    Vec3f computeColor(Sphere sphereList[], int sphereNum,  Vec3f light);
};

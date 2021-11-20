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

Vec3<float> Ray::computeColor(std::vector<Sphere> sphere_list,std::vector<Triangle> triangle_list, Vec3<float> light){
    float  min_t = 9000;
    int min_i = -1;
    float t;//intersection t
    Vec3<float> color;
    Vec3<float> normal;
    Vec3<float> intersection_point;
    Vec3<float> point_to_light;
    int object_index = -1; // Sphere -> -1 , Triangle -> 0,
    for (int i = 0; i < sphere_list.size(); i++){
        t = intersectRayWithSphere(sphere_list[i]);
        //printf("sphere %d t: %f, minT: %f   \n", i , t , minT);
        if (t < min_t && t >= 0){ //could be t >= 0
            // TODO: HOW TO DETERMINE COLOR
            color = sphere_list[i].material.ambient;
            min_i = i;
            min_t = t;
            object_index = -1;
        }
    }
    for (int i = 0; i < triangle_list.size(); i++){
        t = intersectRayWithTriangle(triangle_list[i]);
        if (t < min_t && t >= 0){ //could be t >= 0
            // TODO: HOW TO DETERMINE COLOR
            color = sphere_list[i].material.ambient;
            min_i = i;
            min_t = t;
            object_index = 0;
        }
    }
    if(min_i != -1){
        intersection_point =  this->o.addVector(this->d.multScalar(min_t));
        point_to_light = light.addVector(intersection_point.multScalar(-1));
        normal = intersection_point.addVector(sphere_list[min_i].center.multScalar(-1));
        point_to_light = point_to_light.normalize();
        normal = normal.normalize();
        float dotProduct = point_to_light.dot(normal);
        if (dotProduct > 0)
            color = color.multScalar(dotProduct);
        else
            color.x = color.y = color.z = 0;
    }
    return color;
}

float Ray::intersectRayWithSphere(Sphere sphere){
    float A, B, C;
    float delta;
    Vec3<float> c  = sphere.center;
    float t, t1, t2;
//    B = 2*this->d.x*(this->o.x-c.x)+2*this->d.y*(this->o.y-c.y)+2*this->d.z*(this->o.z-c.z);
    B = this->d.multScalar(2).dot(this->o.subtVector(sphere.center));
//    C = (this->o.x-c.x)*(this->o.x-c.x)+(this->o.y-c.y)*(this->o.y-c.y)+(this->o.z-c.z)*(this->o.z-c.z)-sphere.radius*sphere.radius;
    C = this->o.subtVector(c).dot(this->o.subtVector(c)) - pow(sphere.radius, 2);
//    A = this->d.x*this->d.x+this->d.y*this->d.y+this->d.z*this->d.z;
    A = this->d.dot(this->d);
    delta = B*B - 4*A*C;
    if (delta<0) return -1;
    else if (delta==0){
        t = -B / (2*A);
    }
    else{
        delta = sqrt(delta);
        A = 2*A;
        t1 = (-B + delta) / A;
        t2 = (-B - delta) / A;
        if (t1<t2) t=t1; else t=t2;
    }
    return t;
}


float Ray::calculateDeterminant(const std::vector<std::vector<float>> & matrix ){
    float matrix_00 = matrix[0][0]*(matrix[1][1]*matrix[2][2]-matrix[1][2]*matrix[2][1]);
    float matrix_10 = matrix[1][0]*(matrix[0][1]*matrix[2][2]-matrix[0][2]*matrix[2][1]);
    float matrix_20 = matrix[2][0]*(matrix[0][1]*matrix[1][2]-matrix[1][1]*matrix[1][2]);
    return matrix_00 - matrix_10 + matrix_20;
}

float Ray::intersectRayWithTriangle(Triangle triangle){
    //Barycentric Method with Applying Cramer's Rule:
    std::vector<std::vector<float>> A(3, std::vector<float>(3));
    std::vector<std::vector<float>> B(3, std::vector<float>(3));
    std::vector<std::vector<float>> G(3, std::vector<float>(3));
    std::vector<std::vector<float>> T(3, std::vector<float>(3));
    float det_A, det_B, det_G, det_T, beta, gama, t;
    A[0] = {triangle.coords[0].x - triangle.coords[1].x , triangle.coords[0].x - triangle.coords[2].x, this->d.x};
    A[1] = {triangle.coords[0].y - triangle.coords[1].y, triangle.coords[0].y - triangle.coords[2].y, this->d.y};
    A[2] = {triangle.coords[0].z - triangle.coords[1].z, triangle.coords[0].z - triangle.coords[2].z, this->d.z};
    det_A = calculateDeterminant(A);
    if(det_A==0){
        return -1;
    }
    B[0] = {triangle.coords[0].x - this->o.x, triangle.coords[0].x - triangle.coords[2].x, this->d.x};
    B[1] = {triangle.coords[0].y - this->o.y, triangle.coords[0].y - triangle.coords[2].y, this->d.y};
    B[2] = {triangle.coords[0].z - this->o.z, triangle.coords[0].z - triangle.coords[2].z, this->d.z};
    det_B = calculateDeterminant(B);
    G[0] = {triangle.coords[0].x - triangle.coords[1].x , triangle.coords[0].x - this->o.x, this->d.x};
    G[1] = {triangle.coords[0].y - triangle.coords[1].y, triangle.coords[0].y - this->o.y, this->d.y};
    G[2] = {triangle.coords[0].z - triangle.coords[1].z, triangle.coords[0].z - this->o.z, this->d.z};
    det_G = calculateDeterminant(G);
    T[0] = {triangle.coords[0].x - triangle.coords[1].x , triangle.coords[0].x - triangle.coords[2].x, triangle.coords[0].x - this->o.x};
    T[1] = {triangle.coords[0].y - triangle.coords[1].y, triangle.coords[0].y -  triangle.coords[2].y, triangle.coords[0].y - this->o.y};
    T[2] = {triangle.coords[0].z - triangle.coords[1].z, triangle.coords[0].z - triangle.coords[2].z, triangle.coords[0].z - this->o.z};
    det_T = calculateDeterminant(T);
    beta = det_B/det_A;
    gama = det_G/det_A;
    t = det_T/det_A;
    if(beta + gama > 1 || beta<0 || gama<0){
        return -1;
    }
    return t;
}

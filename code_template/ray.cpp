//
//


#include "ray.h"
float float_max =std::numeric_limits<float>::max();

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

Ray::Ray(Vec3<float> origin,Vec3<float> direction){
    this->o = origin;
    this->d = direction;
}

Vec3<float> Ray::computeColor( Vec3<int>& background_color,float& shadow_ray_epsilon,
                               Vec3<float>& ambient_light , std::vector<PointLightSource> & point_lights, std::vector<Sphere> & spheres
                             , std::vector<Triangle> & triangles, int recursion_depth){

    Shape * shape = NULL;
    /* INTERSECT RAY WITH ANY SHAPE GET THE CLOSEST */
    float minT = this->intersectRayWithAnyShape(spheres,triangles,shape);
    Vec3<float> intersectionPoint;

    // if NO INTERSECTION
    Vec3<float> finalColor(background_color.x,background_color.y,background_color.z);

    /* AMBIENT COMPONENT */
    if (shape) {
        Vec3<float> material_ambient_reflectance = shape->material.ambient;
        finalColor = ambient_light.multVectorsElementwise(material_ambient_reflectance);

        intersectionPoint = this->o.addVector(this->d.multScalar(minT));

        Vec3<float> surfaceNormal = shape->calculateNormalVector(intersectionPoint);
        surfaceNormal = surfaceNormal.normalize();

        Vec3<float> pointToCameraVector = this->o.subtVector(intersectionPoint);
        pointToCameraVector = pointToCameraVector.normalize();

        for (auto & i : point_lights) {
            PointLightSource point_light = i;
            Vec3<float> pointToLight = point_light.position.subtVector(intersectionPoint);
            pointToLight = pointToLight.normalize();
            /* Shadow Ray Intersection Test*/
            Ray shadow_ray(intersectionPoint.addVector(pointToLight.multScalar(shadow_ray_epsilon)),
                           pointToLight);
            //check shadow ray intersection
            bool is_in_shadow = shadow_ray.intersectShadowRayIsInShadow(spheres,triangles);
            if (is_in_shadow)
                continue;

            /* Calculate Diffuse For all point light sources*/
            float dotProduct = pointToLight.dot(surfaceNormal);// /(sqrt(pointToLight.dot(pointToLight))*sqrt(surfaceNormal.dot(surfaceNormal)));
            // homemade clamp function
            float cosine = std::max((float)0, dotProduct);
            //cosine = std::min(cosine, float(1));

            // inverse square law
            float inverse_square_law_denom = i.position.findEuclidianDistanceSquared(
                    intersectionPoint);
            float inverse_square_law = cosine / inverse_square_law_denom;

            Vec3<float> point_intensity = i.intensity.multScalar(inverse_square_law);

            Material material = shape->material;
            Vec3<float> diffuse_component = point_intensity.multVectorsElementwise(material.diffuse);
            finalColor = finalColor.addVector(diffuse_component);

            /* Calculate Specular For all point light sources */
            Vec3<float> halfVector = pointToLight.addVector(pointToCameraVector);
            halfVector = halfVector.normalize();
            float dotProductHalfVec = surfaceNormal.dot(halfVector);

            //clamp cosine
            float cosineHalfVec = std::max(float(0), dotProductHalfVec);
            cosineHalfVec = std::min(cosineHalfVec, float(1));

            //TODO: check if normal and tolight vectors degree is 0 < x < 90
            float cosineHalfVecPhongExp = pow(cosineHalfVec, material.phong_exponent);
            inverse_square_law = cosineHalfVecPhongExp / inverse_square_law_denom;
            Vec3<float> specular_component = i.intensity.multScalar(inverse_square_law);
            specular_component = specular_component.multVectorsElementwise(material.specular);
            finalColor = finalColor.addVector(specular_component);

            if(material.is_mirror && recursion_depth>0 && (material.mirror.x != 0 || material.mirror.y !=0 || material.mirror.z != 0 )){
                float dotProductMirror = surfaceNormal.dot(pointToCameraVector);
                dotProductMirror = std::max(float(0), dotProductMirror);
                dotProductMirror = std::min(dotProductMirror, float(1));

                Vec3<float> mirrorVector = surfaceNormal.multScalar(2*dotProductMirror);
                mirrorVector = mirrorVector.subtVector(pointToCameraVector);
                mirrorVector = mirrorVector.normalize();

                Ray mirror_ray = Ray(intersectionPoint.addVector(mirrorVector.multScalar(shadow_ray_epsilon)), mirrorVector);
                //Vec3<float> no_light = {0,0,0};
                Vec3<float> reflected = mirror_ray.computeColor(background_color, shadow_ray_epsilon, ambient_light, point_lights, spheres, triangles, recursion_depth-1);
                reflected = reflected.multVectorsElementwise(material.mirror);

                finalColor = finalColor.addVector(reflected);
            }


        }
    }

    finalColor.x = std::min(float(255),finalColor.x );
    finalColor.y = std::min(float(255),finalColor.y );
    finalColor.z = std::min(float(255),finalColor.z );

    return finalColor;
}

float Ray::intersectRayWithAnyShape(std::vector<Sphere>& spheres , std::vector<Triangle> & triangles, Shape* & shape_ptr  ){
    float t= -1  ;
    float minT = float_max;
    //check intersection
    for(auto & sphere: spheres){
        t = intersectRayWithSphere(sphere);
        if (t < minT && t >= 0){ //could be t >= 0
            //color = spheres.at(i).color;
            minT = t;
            shape_ptr = &sphere;
        }

    }

    for(auto & triangle: triangles){
        t = intersectRayWithTriangle(triangle);
        if (t < minT && t >= 0){ //could be t >= 0
            minT = t;
            shape_ptr = &triangle;
        }
    }
    return minT;
}

bool Ray::intersectShadowRayIsInShadow(std::vector<Sphere>& spheres , std::vector<Triangle> & triangles ){
    float minT_shadow = float_max;
    for ( auto & sphere : spheres) {
        float t_shadow = this->intersectRayWithSphere(sphere);
        if (t_shadow < minT_shadow && t_shadow >= 0) {
            return true;
        }
    }
    for ( auto & triangle : triangles) {
        float t_shadow = this->intersectRayWithTriangle(triangle);
        if (t_shadow < minT_shadow && t_shadow >= 0) {
            return true;
        }
    }
    return false;
}

float Ray::intersectRayWithSphere( Sphere& sphere){
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

float Ray::calculateDeterminant(const std::vector<std::vector<float>> & matrix ){
    float matrix_00 = matrix[0][0]*(matrix[1][1]*matrix[2][2]-matrix[1][2]*matrix[2][1]);
    float matrix_10 = matrix[1][0]*(matrix[0][1]*matrix[2][2]-matrix[0][2]*matrix[2][1]);
    float matrix_20 = matrix[2][0]*(matrix[0][1]*matrix[1][2]-matrix[1][1]*matrix[0][2]);
    return matrix_00 - matrix_10 + matrix_20;
}

float Ray::intersectRayWithTriangle(Triangle& triangle){

//    BFC  calculating the dot product of the ray direction with
//    the normal vector of the triangle. If the sign of the result is positive, then that triangle is
//    ignored.

    float  dot_product = this->d.dot(triangle.normal);
    if (dot_product > 0 )
        return -1;

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
    if((beta + gama) > 1 || beta<0 || gama<0 || t<0){
        return -1;
    }
    return t;
}



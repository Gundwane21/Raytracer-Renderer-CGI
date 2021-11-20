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

Ray::Ray(Vec3<float> origin,Vec3<float> direction){
    this->o = origin;
    this->d = direction;
}

Vec3<float> Ray::computeColor(std::vector<Sphere> spheres, Vec3<int> background_color,float shadow_ray_epsilon,
    Vec3<float> ambient_light , std::vector<Material> materials , std::vector<PointLightSource> point_lights,
    std::vector<Vec3<float>> vertices  ){
    float  minT = 90000; //std::numeric_limits<float>::max();
    int minI = -1;
    float t;//intersection t

   Vec3<float> intersectionPoint;

    //check intersection
    for (int i = 0 ; i < spheres.size() ; i++){
        t = this->intersectRayWithSphere( spheres.at(i),vertices);
        if (minT != 90000)
            printf("sphere %d t: %f, minT: %f   \n", i , t , minT);
        if (t < minT && t >= 0){ //could be t >= 0
            //color = spheres.at(i).color;
            minI = i;
            minT = t;
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

    // if NO INTERSECTION
    Vec3<float> finalColor(background_color.x,background_color.y,background_color.z);

    /* AMBIENT COMPONENT */
    //  TODO: check if intersection exists
    //  if( intersection_exists ){}
    if (minI != -1 ){

        Vec3<float> material_ambient_reflectance = materials.at(spheres.at(minI).material_id - 1).ambient_reflectance;
        finalColor =  ambient_light.multVectorsElementwise(material_ambient_reflectance);

        intersectionPoint =  this->o.addVector(this->d.multScalar(minT));

        Vec3<float> sphere_center_vertex = vertices.at(spheres.at(minI).center_vertex_id-1);
        Vec3<float> surfaceNormal;
        surfaceNormal = this->calculateNormalVec(intersectionPoint,sphere_center_vertex);
        surfaceNormal = surfaceNormal.normalize();

        Vec3<float> pointToCameraVector = this->o.addVector(intersectionPoint.multScalar(-1));
        pointToCameraVector = pointToCameraVector.normalize();


        for (int i = 0 ;  i < point_lights.size() ; i++){
            Vec3<float> pointToLight = point_lights.at(i).position.addVector(intersectionPoint.multScalar(-1));
            pointToLight = pointToLight.normalize();

            /* Shadow Ray Intersection Test*/

            Ray shadow_ray( intersectionPoint.addVector(surfaceNormal.multScalar(shadow_ray_epsilon))  , pointToLight );

            //check intersection
            float minT_shadow = 9000;
            bool is_in_shadow = false;
            for (int i = 0 ; i < spheres.size() ; i++){
                float t_shadow = shadow_ray.intersectRayWithSphere( spheres.at(i),vertices);
                if (minT_shadow != 90000 && t_shadow >= 0 ) {
                    printf("sphere %d t: %f, minT: %f   \n", i, t, minT);
                    is_in_shadow = true;
                    break;
                }
            }
            if(is_in_shadow)
                continue;

            /* Calculate Diffuse For all point light sources*/
            float dotProduct  = pointToLight.dot(surfaceNormal);
            // homemade clamp function
            float cosine = std::max(float(0),dotProduct);
            cosine = std::min(cosine, float(1));

            // inverse square law
            float inverse_square_law_denom = point_lights.at(i).position.findEuclidianDistanceSquared(intersectionPoint);
            float inverse_square_law =  cosine / inverse_square_law_denom;
            Vec3<float> point_intensity = point_lights.at(i).intensity.multScalar(inverse_square_law);

            Material material = materials.at(spheres.at(i).material_id - 1);
            Vec3<float> diffuse_component = point_intensity.multVectorsElementwise( material.diffuse_reflectance );
            finalColor = finalColor.addVector(diffuse_component);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /* Calculate Specular For all point light sources */
            Vec3<float> halfVector = pointToLight.addVector(pointToCameraVector);
            halfVector = halfVector.normalize();
            float dotProductHalfVec = surfaceNormal.dot(halfVector);

            //clamp cosine
            float cosineHalfVec =std::max(float(0),dotProductHalfVec);
            cosineHalfVec = std::min(cosineHalfVec,float(1));

            //TODO: check if normal and tolight vectors degree is 0 < x < 90
            float cosineHalfVecPhongExp = pow(cosineHalfVec,material.phong_exponent);
            inverse_square_law = cosineHalfVecPhongExp / inverse_square_law_denom ;

            Vec3<float> specular_component = point_lights.at(i).intensity.multScalar(inverse_square_law);
            specular_component = specular_component.multVectorsElementwise( material.specular_reflectance);
            finalColor = finalColor.addVector(specular_component);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }
    }
/*
   if(minI != -1) {
       intersectionPoint =  this->o.addVector(this->d.multScalar(minT));
       pointToLight = light.addVector(intersectionPoint.multScalar(-1));
       normal = intersectionPoint.addVector(sphereList[minI].center.multScalar(-1));
       pointToLight = pointToLight.normalize();
       normal = normal.normalize();

       float dotProduct = pointToLight.dot(normal);
       if (dotProduct > 0)
           color = color.multScalar(dotProduct);
       else
           color.x = color.y = color.z = 0;
   }*/
   return finalColor;

}


float Ray::intersectRayWithSphere( Sphere sphere, std::vector<Vec3<float>> vertices){
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

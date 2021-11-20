//
// Created by Umur Görkem Karaduman on 2021-11-18.
//

#ifndef GRAPH_HW1_VEC3_H
#define GRAPH_HW1_VEC3_H

#include "parser.h"
#include <typeinfo>
template<typename T>
class Vec3{
public:
    T x, y, z;
    Vec3();
    Vec3(T x, T y, T z);
    Vec3(const parser::Vec3f &);
    Vec3 multScalar(float scalar) const;
    Vec3 addVector(const Vec3 vec2) const;
    Vec3 subtVector(const Vec3 vec2) const;
    float dot(Vec3 vec2);
    Vec3 cross(Vec3 vec2);
    Vec3 normalize();
};


template <class T>
Vec3<T>::Vec3(){
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

template <class T>
Vec3<T>::Vec3(const parser::Vec3f & vec){
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
}

template <class T>
Vec3<T>::Vec3(T x, T y, T z){
    this->x = x;
    this->y = y;
    this->z = z;
}

template <class T>
Vec3<T> Vec3<T>::multScalar(float scalar) const {
    Vec3 result;
    result.x = this->x *scalar;
    result.y = this->y *scalar;
    result.z = this->z * scalar;
    return result;
}

template <class T>
Vec3<T> Vec3<T>::addVector(const Vec3<T> vec2) const{
    Vec3 result;
    result.x = this->x + vec2.x;
    result.y = this->y + vec2.y;
    result.z = this->z + vec2.z;
    return result;
}

template <class T>
Vec3<T> Vec3<T>::subtVector(const Vec3<T> vec2) const{
    Vec3 result;
    result.x = this->x - vec2.x;
    result.y = this->y - vec2.y;
    result.z = this->z - vec2.z;
    return result;
}

template <class T>
float Vec3<T>::dot(Vec3<T> vec2){
    return this->x * vec2.x + this->y * vec2.y + this->z * vec2.z;
}

template <class T>
Vec3<T> Vec3<T>::cross(Vec3<T> vec2){
    Vec3 result;
    result.x = this->y*vec2.z - this->z*vec2.y;
    result.y = this->z*vec2.x - this->x*vec2.z;
    result.z = this->x*vec2.y - this->y*vec2.x;
    return result;
}

template <class T>
Vec3<T> Vec3<T>::normalize(){
    return multScalar(1.0/sqrt(this->dot(*this)));
}


//class Vec3i: public Vec3{
//public:
//    int x,y,z;
//    Vec3i();
//    Vec3i(int x, int y, int z);
//};
//
//class Vec3f: public Vec3{
//public:
//    float x,y,z;
//    Vec3f();
//    Vec3f(float x, float y, float z);
//};


#endif //GRAPH_HW1_VEC3_H

//
//

#ifndef GRAPH_HW1_SHAPE_H
#define GRAPH_HW1_SHAPE_H


#include "utility.h"

class Shape {
public:
    Material material;
    Shape(parser::Material material);
    virtual Vec3<float> calculateNormalVector(Vec3<float> &) = 0;
};


#endif //GRAPH_HW1_SHAPE_H

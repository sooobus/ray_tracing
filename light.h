#ifndef LIGHT_H
#define LIGHT_H

#pragma once
#include "geom.h"
class PointLightSource{
public:
    PointLightSource(ThreeDVector pos_, double power_) : pos(pos_), power(power_) {};
    ThreeDVector pos;
    double power; //в канделах
};

#endif // LIGHT_H

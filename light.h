#ifndef LIGHT_H
#define LIGHT_H

#pragma once
#include "geom.h"
class PointLightSource{
public:
    PointLightSource(ThreeDVector pos_, long double power_) : pos(pos_), power(power_) {};
    ThreeDVector pos;
    long double power; //в канделах
};

#endif // LIGHT_H

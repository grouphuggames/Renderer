#pragma once
#include "basics.h"

class vec4
{
public:
    vec4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
    vec4(f32 _x, f32 _y, f32 _z, f32 _w) : x(_x), y(_y), z(_z), w(_w) {}

    f32 x, y, z, w;
};

#pragma once
#include "basics.h"

class vec2
{
public:
    vec2() : x(0.f), y(0.f) {}
    vec2(f32 _x, f32 _y) : x(_x), y(_y) {}

    friend vec2 operator+(const vec2& left, const vec2& right)
    {
        return vec2(left.x + right.x, left.y + right.y);
    }

    friend vec2 operator-(const vec2& left, const vec2& right)
    {
        return vec2(left.x - right.x, left.y - right.y);
    }

    friend vec2 operator*(const f32 left, const vec2& right)
    {
        return vec2(right.x * left, right.y * left);
    }

    friend bool operator==(const vec2& a, const vec2 b)
    {
        return (a.x == b.x && a.y == b.y);
    }

    f32 x, y;
};

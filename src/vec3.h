#pragma once
#include "basics.h"
#include <cmath>

class vec3
{
public:
    vec3() : x(0.f), y(0.f), z(0.f) {}
    vec3(f32 val) : x(val), y(val), z(val) {}
    vec3(f32 _x, f32 _y, f32 _z) : x(_x), y(_y), z(_z) {}

    static f32 length(const vec3& vec)
    {
        return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }

    static vec3 normalize(const vec3& vec)
    {
        f32 k = length(vec);
        return vec3(vec.x / k, vec.y / k, vec.z / k);
    }

    static f32 dot(const vec3& u, const vec3& v)
    {
        return u.x * v.x + u.y * v.y + u.z * v.z;
    }

    static vec3 cross(const vec3& u, const vec3& v)
    {
        return vec3(u.y * v.z - u.z * v.y, -(u.x * v.z - u.z * v.x), u.x * v.y - u.y * v.x);
    }

    friend bool operator==(const vec3& a, const vec3 b)
    {
        return (a.x == b.x && a.y == b.y && a.z == b.z);
    }

    friend vec3 operator*(const f32 left, const vec3& right)
    {
        return vec3(right.x * left, right.y * left, right.z * left);
    }

    vec3 operator*(const vec3& other)
    {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;

        return *this;
    }

    friend vec3 operator+(const vec3& left, const vec3& right)
    {
        return vec3(left.x + right.x, left.y + right.y, left.z + right.z);
    }

    friend vec3 operator-(const vec3& left, const vec3& right)
    {
        return vec3(left.x - right.x, left.y - right.y, left.z - right.z);
    }

    vec3& operator+=(const vec3& other)
    {
        *this = *this + other;
        return *this;
    }

    vec3& operator-=(const vec3& other)
    {
        *this = *this - other;
        return *this;
    }

    f32 x, y, z;
};

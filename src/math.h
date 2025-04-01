#pragma once
#include <cmath>
#include "vec2.h"
#include "vec4.h"
#include "mat4.h"

const f32 pi = 3.14159265f;
const f32 deg_to_rad_constant = pi / 180.f;

inline f32 DegreesToRadians(f32 degrees) { return degrees * deg_to_rad_constant; }

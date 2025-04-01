#pragma once
#include "vec3.h"
vec3 up_vector = vec3(0.f, 1.f, 0.f);

struct Camera
{
  vec3 position = vec3(0.f, 0.f, 3.f);
  vec3 target = vec3(0.f, 0.f, 0.f);
  vec3 direction = vec3::normalize(position - target);
  vec3 right = vec3::normalize(vec3::cross(up_vector, direction));
  vec3 up = vec3::cross(direction, right);
  vec3 front = vec3(0.f, 0.f, -1.f);
  f32 speed = 0.05f;
};

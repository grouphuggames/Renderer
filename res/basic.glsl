#shader vertex
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

out vec3 _normal;
out vec3 fragment_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.f);
	_normal = normal;
	fragment_position = vec3(model * vec4(pos, 1.f));
}


#shader fragment
#version 330 core

out vec4 frag_color;

in vec3 _normal;
in vec3 fragment_position;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 view_position;

void main()
{
  float ambient_strength = 0.1f;
  vec3 ambient = ambient_strength * light_color;

  vec3 norm = normalize(_normal);
  vec3 light_direction = normalize(light_position - fragment_position);

  float diff = max(dot(norm, light_direction), 0.f);
  vec3 diffuse = diff * light_color;

  float specular_strength = 0.5f;
  vec3 view_direction = normalize(view_position - fragment_position);
  vec3 reflect_direction = reflect(-light_direction, norm);

  float spec = pow(max(dot(view_direction, reflect_direction), 0.f), 32);
  vec3 specular = specular_strength * spec * light_color;

  vec3 result = (ambient + diffuse + specular) * object_color;
  frag_color = vec4(result, 1.f);
}
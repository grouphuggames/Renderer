#shader vertex
#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.f);
}


#shader fragment
#version 330 core

out vec4 frag_color;

void main()
{
  frag_color = vec4(1.f);
}

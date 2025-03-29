#shader vertex
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex_coord;

out vec2 texture_coordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.f);
	texture_coordinates = tex_coord;
}


#shader fragment
#version 330 core

out vec4 frag_color;

in vec2 texture_coordinates;

uniform sampler2D _texture;

void main()
{
	frag_color = texture(_texture, texture_coordinates);
}
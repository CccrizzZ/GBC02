#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;
layout(location = 2) in vec2 vertex_texture;
layout(location = 3) in vec3 vertex_normal;


out vec3 colour;
out vec2 texCoord;
out vec3 normal;

// Values that stay constant for the whole mesh.
uniform mat4 mvp;
uniform mat4 view;
uniform mat4 projection;



void main()
{
	gl_Position = projection * view * mvp * vec4(vertex_position, 1.0f);
	colour = vertex_colour;
	texCoord = vertex_texture;
	texCoord = vec2(texCoord.x, 1.0f - texCoord);
	normal = mat3(transpose(inverse(mvp))) * vertex_normal;
}
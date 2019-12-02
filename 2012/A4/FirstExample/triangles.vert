#version 410 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_texture;
layout(location = 2) in vec3 vertex_normal;

out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;


// Values that stay constant for the whole mesh.
uniform highp mat4 MVP;
uniform highp mat4 V;
uniform highp mat4 M;



void main()
{
	gl_Position = MVP * vec4(vertex_position, 1.0f);
	UV = vertex_texture;
	UV = vec2(UV.x, 1.0f - UV.y);
	Position_worldspace = vec3(M * vec4(vertex_position, 1.0f));

	vec3 vertexPosition_CameraSpace = vec3(V * M * vec4(vertex_position,1));
	EyeDirection_cameraspace = vec3(0,0,0);

	Normal_cameraspace = vec3(V * M * vec4(vertex_normal, 0));
}
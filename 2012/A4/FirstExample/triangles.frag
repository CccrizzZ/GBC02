#version 430 core

in vec3 colour;
in vec2 texCoord;
in vec3 normal;
out vec4 frag_colour;

uniform sampler2D texture0;
uniform vec3 ambientColor;
uniform float ambientStrength;
uniform vec3 lightDirection;
uniform vec3 diffuseColor;
uniform float diffuseStrength;
 
void main()
{
	vec4 ambient = vec4(ambientColor, 1.0f) * ambientStrength;
	frag_colour = texture(texture0, texCoord) * vec4(colour, 1.0f) * ambient;


}
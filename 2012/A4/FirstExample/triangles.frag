#version 410 core

in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;

out vec4 frag_colour;

#ifndef NUM_POINT_LIGHTS
	#define NUM_POINT_LIGHTS 2
#endif


struct Light 
{
	vec3 Color;
	vec3 Position;
	float Strength;
	float falloffStart;
	float falloffEnd;
};

uniform sampler2D diffuseTexture;
uniform Light pointLights[NUM_POINT_LIGHTS];

uniform mat4 V;

vec3 calcPointLights()
{
	vec3 totalLight = vec3(0);

	for(int i=0; i<NUM_POINT_LIGHTS; i++)
	{
		float distance = length(pointLights[i].Position - Position_worldspace);
	
		if(distance <= pointLights[i].falloffEnd)
		{
			vec3 n = normalize(Normal_cameraspace);

			vec3 LightPosition_cameraspace = (V * vec4(pointLights[i].Position,1)).xyz;

			vec3 LightDirection = LightPosition_cameraspace + EyeDirection_cameraspace;

			vec3 l = normalize(LightDirection);
			
			float diff = max(dot(n,l), 0.0f);
			vec3 diffuse = pointLights[i].Color * pointLights[i].Strength * diff * ((pointLights[i].falloffEnd-distance)/(pointLights[i].falloffStart));
			totalLight += diffuse;

			// specular strength
			float specStrength = 5;
			vec3 E = normalize(EyeDirection_cameraspace);

			vec3 R = reflect(-l, n);

			float cosAlpha = max(dot(E, R), 0);
			vec3 specular = pointLights[i].Color * specStrength * pow(cosAlpha, 32) / (distance*distance);
			totalLight+=specular;
		}
	}
	return totalLight;
}


void main()
{
	vec3 ambient = vec3(0.2, 0.2, 0.2);
	frag_colour =  vec4((ambient+calcPointLights()), 1) * texture(diffuseTexture, UV);
}
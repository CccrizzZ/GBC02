#version 430 core

in vec3 fragPos;
in vec2 texCoord;
in vec3 normal;
in vec3 colour;
out vec4 frag_colour;

struct Light
{
	vec3 diffuseColour;
	float diffuseStrength;
};

struct AmbientLight
{
	vec3 ambientColour;
	float ambientStrength;
};

struct DirectionalLight 
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight point;
	vec3 direction;
	float edge;
};

struct Material
{
	float specularStrength;
	float shininess;
};

uniform sampler2D texture0;
uniform vec3 eyePosition;

uniform AmbientLight aLight;
uniform DirectionalLight dLight;
uniform PointLight pLight;
uniform PointLight pLight2;
uniform SpotLight sLight;
uniform Material mat;

vec4 calcLightByDirection(Light l, vec3 dir)
{
	float diffuseFactor = max(dot(normalize(normal),normalize(dir)), 0.0f);
	vec4 diffuse = vec4(l.diffuseColour, 1.0f) * l.diffuseStrength * diffuseFactor;

	vec4 specular = vec4(0,0,0,0);
	if (diffuseFactor > 0.0f && l.diffuseStrength > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - fragPos);
		vec3 reflectedVertex = normalize(reflect(dir, normalize(normal)));

		float specularFactor = dot(fragToEye, reflectedVertex);
		if (specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, mat.shininess);
			specular = vec4(l.diffuseColour * mat.specularStrength * specularFactor, 1.0f);
		}
	}

	return (diffuse + specular);
}

vec4 calcDirectionalLight()
{
	return calcLightByDirection(dLight.base, dLight.direction);
}

vec4 calcPointLight(PointLight p)
{
	vec3 direction = fragPos - p.position;
	float distance = length(direction);
	direction = normalize(direction);
		
	vec4 colour = calcLightByDirection(p.base, direction);
	float attenuation = p.exponent * distance * distance +
						p.linear * distance +
						p.constant;
	
	return (colour / attenuation);
}

vec4 calcSpotLight(SpotLight s)
{
	vec4 colour = vec4(0,0,0,0);
	vec3 rayDirection = normalize(fragPos - s.point.position);
	float slFactor = dot(rayDirection, s.direction);
	if (slFactor > s.edge)
		colour = calcPointLight(s.point); 
		colour *= (1.0f - (1.0f - slFactor)*(1.0f/(1.0f - s.edge)));
	return colour;
}

void main()
{
	vec4 calcColour = vec4(0,0,0,0);
	
	vec4 ambient = vec4(aLight.ambientColour, 1.0f) * aLight.ambientStrength;
	calcColour += ambient;
	calcColour += calcDirectionalLight();
	calcColour += calcPointLight(pLight);
	calcColour += calcPointLight(pLight2);
	calcColour += calcSpotLight(sLight);
	
	frag_colour = texture(texture0, texCoord) * vec4(colour, 0.8f) * calcColour;
}
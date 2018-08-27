#version 330 core

out highp vec4 fragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 UV;

uniform sampler2D diffuseMap;

vec3 lightPos = vec3(0,  150,120);
uniform vec3 lightColor;
uniform bool lighting;


void main()
{
	vec3 N = normalize(Normal);
	vec3 L = normalize(lightPos- FragPos);

	float diff =  max(dot(N, L), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = clamp(diffuse, 0.0, 1.0)  * vec3(.0,1.0,1.0);

	fragColor = vec4(result,1.0);
}

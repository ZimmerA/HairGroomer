/*
* Used for drawing meshes with a diffuse light model
*/
#version 330 core

out vec4 fragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 UV;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform bool lighting;

uniform vec2 brushCenter;
uniform float brushSize;
uniform bool brushVisible;

void main()
{
	vec3 N = normalize(Normal);
	vec3 L = normalize(lightPos - FragPos);
	float diff = max(dot(N, L), 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 result = clamp(diffuse, 0.0, 1.0) * vec3(0.0, 1.0, 1.0);

	fragColor = vec4(result, 1.0);

	// Show paintbrush position on 3D model
	if (brushVisible)
		fragColor.r = 1.0 - min(distance(vec2(UV.x, 1.0 - UV.y), brushCenter) / brushSize, 1.0);
}

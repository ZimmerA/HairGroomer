/*
* Used for generating the hair using a texture and the input data of the growthmesh
*/
#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 50) out;

in VS_OUT {
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec3 worldPos;
} gs_in[];

out vec4 vertColor;
out vec3 dOut; // Direction
out vec3 worldPosOut;

uniform mat4 model;
uniform mat4 vp;

// Hair
uniform sampler2D hairMap;
uniform float maxHairLength;
uniform int numSegments;
uniform vec3 hairColor;
uniform vec3 rootColor;

const float PI = 3.14159265359;
const float maxHairBendAngle = 180.0;

float rad(float deg)
{
	return deg * PI / 180.0;
}

mat3 rotXMatrix(float rad)
{
	mat3 mat = mat3(
		vec3(1.0, 0.0, 0.0),
		vec3(0.0, cos(rad), -sin(rad)),
		vec3(0.0, sin(rad), cos(rad)));

	return transpose(mat);
}

mat3 rotYMatrix(float rad)
{
	mat3 mat = mat3(
		vec3(cos(rad), 0.0, -sin(rad)),
		vec3(0.0, 1.0, 0.0),
		vec3(sin(rad), 0.0, cos(rad)));

	return transpose(mat);
}

// Maps a color with a range from 0 to 1 to a range of -maxHairBendAngle to maxHairBendAngle (in radians)
float reMapColorValue(float colorValue)
{
	float minusPiToPi = colorValue * (2 * rad(maxHairBendAngle)) - rad(maxHairBendAngle);

	return minusPiToPi;
}

// Helper function so a color value of 128 (RGB) is equal to exactly .5 (no hair bend)
float commaSmooth(float value)
{
	float alteredValue = value;

	if ((alteredValue > 0.4999) && (alteredValue < 0.51))
		alteredValue = 0.5;

	return alteredValue;
}

// Calculates a rotation matrix that rotates hair arround the x and y axis (tangent / bitangent)
mat3 calculateHairBendMatrix(float greenChannel, float blueChanel)
{
	greenChannel = commaSmooth(greenChannel);
	blueChanel = commaSmooth(blueChanel);

	float tangentBend = reMapColorValue(greenChannel);
	float bitangentBend = reMapColorValue(blueChanel);

	// Generate rotationMatrices
	mat3 mRotT = rotXMatrix(tangentBend / numSegments);
	mat3 mRotB = rotYMatrix(bitangentBend / numSegments);
	mat3 hairRot = mRotB * mRotT;

	return hairRot;
}

void main()
{
	vec2 uv = gs_in[0].texCoord;
	float hair = texture(hairMap, uv).r;

	// Generate Hair
	vec3 worldPos = gs_in[0].worldPos;
	vec3 worldNorm = gs_in[0].normal;

	worldNorm = normalize(worldNorm);
	vec3 worldTangent = normalize(gs_in[0].tangent);
	vec3 worldBiTangent = normalize(gs_in[0].bitangent);

	// Tangent space matrix
	mat3 tbn = mat3(
		worldTangent,
		worldBiTangent,
		worldNorm);

	// Hair root
	vertColor = vec4(rootColor, 1.0);
	vec3 d = vec3(0.0, 0.0, 1.0);
	dOut = tbn * d;
	worldPosOut = worldPos;
	gl_Position = vp * vec4(worldPos, 1.0);
	EmitVertex();

	// No actual hair, we still need to generate a hair with the length 0 for hairworks to work properly
	if (hair < 0.01)
	{
		// Hair segments
		for (int i = 0; i < numSegments; ++i)
		{
			vertColor = vec4(hairColor, 1.0);
			d = d;
			dOut = tbn * d;
			worldPos = worldPos;
			worldPosOut = worldPos;
			gl_Position = vp * vec4(worldPos, 1.0);
			EmitVertex();
		}

	}
	else // Actual hair
	{
		mat3 mHairRot = calculateHairBendMatrix(texture(hairMap, uv).g, texture(hairMap, uv).b);
		float hairlength = mix(0,maxHairLength,hair);
		// Hair segments
		for (int i = 0; i < numSegments; ++i)
		{
			vertColor = vec4(hairColor, 1.0);
			d = mHairRot * d;
			dOut = tbn * d;
			worldPos = worldPos + hairlength / numSegments * tbn * d;
			worldPosOut = worldPos;
			gl_Position = vp * vec4(worldPos, 1.0);
			EmitVertex();
		}
	}

	EndPrimitive();
}

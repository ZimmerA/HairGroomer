/*
* Transfers the relevant hair generation data to the geometry shader
*/
#version 330 core

in vec3 aVertex;
in vec3 aNormal;
in vec3 aTangent;
in vec3 aBitangent;
in vec2 aUV;

out VS_OUT {
    vec2 texCoord;
    vec2 uv;
    vec3 normal;
	vec3 tangent;
	vec3 bitangent;
    vec3 worldPos;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 mvp;
void main()
{
    vs_out.normal = vec3(model* vec4(aNormal,0.0));
	vs_out.tangent = vec3(model * vec4(aTangent,0.0));
	vs_out.bitangent = vec3(model * vec4(aBitangent, 0.0));
    vs_out.worldPos = vec3(model * vec4(aVertex,1.0));
    vs_out.texCoord = vec2(aUV.x, 1-aUV.y);
    vs_out.uv = aUV;

    gl_Position = mvp * vec4(aVertex, 1.0f);
}
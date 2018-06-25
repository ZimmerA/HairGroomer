#version 330 core

layout (location = 0) in vec3 aVertex;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out VS_OUT {
    vec2 texCoord;
    vec2 uv;
    vec3 normal;
    vec3 worldPos;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 mvp;
void main()
{
    vs_out.normal = vec3(model* vec4(aNormal,0.0));
    vs_out.worldPos = vec3(model * vec4(aVertex,1.0));
    vs_out.texCoord = vec2(aUV.x,1-aUV.y);
    vs_out.uv = aUV;

    gl_Position = mvp * vec4(aVertex, 1.0f);
}
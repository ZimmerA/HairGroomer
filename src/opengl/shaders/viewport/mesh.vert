#version 330 core

in vec3 aVertex;
in vec3 aNormal;
in vec3 aTangent;
in vec4 aBitangent;
in vec2 aUV;

out vec3 Normal;
out vec3 FragPos;
out vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 mvp;


void main()
{
   Normal = vec3(model * vec4(aNormal,1.0f));
   FragPos = vec3(model * vec4(aVertex,1.0f));
   UV = aUV;
   
   gl_Position =  mvp * vec4(aVertex, 1.0f);
}

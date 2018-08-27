#version 330 core
in vec2 aVertex;
in vec2 aUV;

out vec2 UV;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(aVertex.x,aVertex.y,0.0,1.0);
    UV = aUV;
}

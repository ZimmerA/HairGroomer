/*
* Used for drawing the texture of the painting framebuffer
*/
#version 330 core
in vec2 aVertex;
in vec2 aUV;

out vec2 UV;

void main()
{
    gl_Position = vec4(aVertex.x,aVertex.y,0.0,1.0);
    UV = aUV;
}

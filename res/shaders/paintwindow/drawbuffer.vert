/*
* Used for drawing the texture of the painting framebuffer
*/
#version 330 core

out vec2 UV;

in vec2 aVertex;
in vec2 aUV;

void main()
{
	gl_Position = vec4(aVertex.x, aVertex.y, 0.0, 1.0);
	UV = aUV;
}

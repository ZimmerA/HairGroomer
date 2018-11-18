/*
* Used for drawing the paintbrush with the set intensity
*/
#version 330 core

out vec2 UV;

in vec2 aVertex;
in vec2 aUV;

uniform mat4 model;

void main()
{
	gl_Position = model * vec4(aVertex.x, aVertex.y, 0.0, 1.0);
	UV = aUV;
}

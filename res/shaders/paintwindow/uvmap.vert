/*
* Used for unwraping the texcoords to a flat surface
* to visualize the uv map of the mesh.
*/
#version 330 core

in vec2 aUV;

void main()
{
	// Unwrap the texcoords
	vec3 uvMap = vec3(2.0 * aUV.x - 1.0, -(2.0 * aUV.y - 1.0), 0.0);
	gl_Position =  vec4(uvMap, 1.0);
}

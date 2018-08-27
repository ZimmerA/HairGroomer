/*
* Used for unwraping the texcoords to a flat surface
* to visualize the uv map of the mesh.
*/
#version 330 core
in vec3 aVertex;
in vec3 aNormal;
in vec2 aUV;

void main()
{
	// Unwrap the texcoords
    vec3 uvMap = vec3(2 * aUV.x - 1 ,-(2 * aUV.y-1) , 0);
    gl_Position =  vec4(uvMap, 1.0f);
}

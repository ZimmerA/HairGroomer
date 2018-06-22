#version 330

in vec3 aVertex;
in vec3 aNormal;
in vec2 aUV;

void main()
{
    vec3 uvMap = vec3(2 * aUV.x - 1 ,-(2 * aUV.y-1) , 0);
    gl_Position =  vec4(uvMap, 1.0f);
}

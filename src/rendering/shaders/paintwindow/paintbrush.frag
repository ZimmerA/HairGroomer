/*
* Used for drawing the paintbrush with the set intensity
*/
#version 330 core

out highp vec4 fragColor;
in vec2 UV;

uniform sampler2D screenTexture;
uniform float intensity;

void main()
{
    fragColor = texture2D(screenTexture,UV) * vec4(vec3(intensity),1.0);
}

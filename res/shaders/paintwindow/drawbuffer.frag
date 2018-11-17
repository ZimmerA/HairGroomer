/*
* Used for drawing the texture of the painting framebuffer
*/
#version 330 core
out highp vec4 fragColor;
in vec2 UV;

uniform sampler2D screenTexture;

void main()
{
    fragColor = texture(screenTexture,UV);
}

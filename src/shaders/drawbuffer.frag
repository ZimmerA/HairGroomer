#version 150

out highp vec4 fragColor;
in vec2 UV;

uniform sampler2D screenTexture;

void main()
{
    fragColor = texture2D(screenTexture,UV);
    //fragColor = vec4(UV, 0.0,1.0);
}

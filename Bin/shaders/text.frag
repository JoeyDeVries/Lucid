#version 330 core
out vec4 fragColor;

in VS_OUT
{
    vec2 TexCoords;
    vec4 Color;
} fs_in;

uniform sampler2D fontAtlas;

const float width = 0.5;
const float edge = 0.15;

void main()
{
    float distance = 1.0 - texture(fontAtlas, fs_in.TexCoords).a;
    float alpha = 1.0 - smoothstep(width, width + edge, distance);

    fragColor = vec4(fs_in.Color.rgb, min(alpha, fs_in.Color.a));
    // fragColor = vec4(1.0, 1.0, 1.0, alpha);
}
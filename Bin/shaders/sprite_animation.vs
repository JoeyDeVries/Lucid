#version 330 core
layout (location = 0) in vec4 vertex;

uniform mat4 projection;
uniform mat4 model;

const int MAX_NR_FRAMES = 16;
uniform int NrFrames;
uniform vec4 AnimationData; // (XOffset, YOffset, Width, Height)

out VS_OUT
{
    vec2 TexCoords;
    vec3 FragPos;
} vs_out;


void main()
{
    vec4 fragPos = model * vec4(vertex.xy, 0.0, 1.0);
    gl_Position = projection * fragPos;
    vs_out.TexCoords = vertex.zw;
    vs_out.FragPos = fragPos.xyz;
}
#version 330 core
layout (location = 0) in vec4 vertex;

out VS_OUT
{
    vec2 TexCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 model;

void main()
{
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
    vs_out.TexCoords = vertex.zw;
}
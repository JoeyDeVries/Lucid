#version 330 core
layout (location = 0) in vec4 vertex; // <position, texCoords>
layout (location = 1) in vec4 color; 

out VS_OUT
{
    vec2 TexCoords;
    vec4 Color;
} vs_out;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    vs_out.TexCoords = vec2(vertex.z, vertex.w);
    vs_out.Color = color;
}


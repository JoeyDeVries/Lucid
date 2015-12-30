#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in vec4 particleInfo; // <vec2 offset, vec2 scale>
layout (location = 2) in vec4 color; 

out vec2 FragPos;
out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vec2 pos = vertex.xy * particleInfo.zw + particleInfo.xy;
    FragPos = pos;
    TexCoords = vertex.zw;
    ParticleColor = color;
    gl_Position = projection * view * vec4(pos, 0.0, 1.0);
}
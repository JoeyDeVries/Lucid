#version 330 core 
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform int effect;

void main()
{
    // coordinates are between [0,1]; convert to [-1,1]
    vec2 position = vertex.xy;
    // position.y = 1.0 - position.y;
    position = position * 2.0 - 1.0;
    gl_Position = vec4(position, 0.0, 1.0);
    TexCoords = vertex.zw;
}
#version 330 core
out vec4 color;

in VS_OUT
{
    vec2 TexCoords;
    vec3 FragPos;
} fs_in;



void main()
{
    color = vec4(1.0);
}
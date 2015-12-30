#version 330 core
in vec2 FragPos;
in vec2 TexCoords;
in vec4 ParticleColor;
out vec4 color;

uniform sampler2D particle;

#pragma include common/lighting.glsl

void main()
{
    // color = (texture(sprite, TexCoords) * ParticleColor);       
    vec4 cDiffuse = texture(particle, TexCoords);
    cDiffuse.rgb *= ParticleColor.rgb;

    // sample per-fragment normal
    vec3 normal = vec3(0.0, 0.0, 1.0);
    // Blinn-Phong lighting    
    const vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    for(int i = 0; i < NrActiveLights; ++i)
    {
        float attenuation = CalcAttenuation(vec3(FragPos, 0.0), Lights[i].Position, Lights[i].Linear);       
        diffuse += attenuation * CalcDiffuse_L(vec3(FragPos, 0.0), normal, Lights[i].Position) * Lights[i].Diffuse;
    }    
    color = vec4((ambient + diffuse) * cDiffuse.rgb, cDiffuse.a - (1.0 - ParticleColor.a));
    // color = vec4(diffuse, 1.0);
    // color = texture(particle, TexCoords);
    // color = vec4(1.0f);
}
#version 330 core
out vec4 color;

in VS_OUT
{
    vec2 TexCoords;
	vec2 TexCoords_NR;
    vec3 FragPos;
} fs_in;

uniform sampler2D tDiffuse;
uniform sampler2D tSpecular;
uniform sampler2D tNormal;
uniform vec4 ColorOverride;

#pragma include common/lighting.glsl

uniform bool EnableLighting;


void main()
{
    vec4 cDiffuse = texture(tDiffuse, fs_in.TexCoords);
    float alpha = cDiffuse.a;
    if(EnableLighting)
    {
        // sample per-fragment normal
        vec3 normal = normalize(vec3(texture(tNormal, fs_in.TexCoords_NR).rgb * 2.0 - 1.0));        
        // Blinn-Phong lighting    
        vec3 ambient = COLOR_AMBIENT;
        vec3 diffuse = vec3(0.0);
        vec3 specular = vec3(0.0);
        for(int i = 0; i < NrActiveLights; ++i)
        {
            float attenuation = CalcAttenuation(fs_in.FragPos, Lights[i].Position, Lights[i].Linear);       
            diffuse += attenuation *  CalcDiffuse_L(fs_in.FragPos, normal, Lights[i].Position) * Lights[i].Diffuse * vec3(ColorOverride);
            specular += attenuation * CalcSpecular_BP(fs_in.FragPos, normal, Lights[i].Position, 8);
        }    
        specular *= texture(tSpecular, fs_in.TexCoords).rgb;
        color = vec4((ambient + diffuse + specular) * cDiffuse.rgb, alpha - (1.0 - ColorOverride.a));
    }
    else
        color = vec4(cDiffuse.rgb * ColorOverride.rgb, alpha - (1.0 - ColorOverride.a));
    // color = vec4(fs_in.TexCoords, 0.0, 1.0);
    // color = vec4(specular * cDiffuse.rgb, alpha);
    // color = cDiffuse;
    // color = vec4(normal, 1.0);
}
#version 330 core
layout (location = 0) in vec4 vertex;

out VS_OUT
{
    vec2 TexCoords;
	vec2 TexCoords_NR; // Non-Reversed
    vec3 FragPos;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform bool animation;
uniform vec4 animationData; // (xOffset, yOffset, width, height)

uniform bool reverse;


void main()
{
    vec4 fragPos = model * vec4(vertex.xy, 0.0, 1.0);
    gl_Position = projection * view * fragPos;
    if(animation)
    {
        // vec2 bottomLeft = animationData.xy;
        // vec2 
		vs_out.TexCoords = vec2(animationData.xy + vertex.zw * vec2(animationData.z, animationData.w));
		vs_out.TexCoords_NR = vs_out.TexCoords;
		if(reverse)
			vs_out.TexCoords = vec2(animationData.xy + vec2(1.0 - vertex.z, vertex.w) * vec2(animationData.z, animationData.w));
    }
    else
    {
		// vs_out.TexCoords = vec2(1.0 - vs_out.TexCoords.x, vs_out.TexCoords.y);
        vs_out.TexCoords = vertex.zw;
        vs_out.TexCoords_NR = vertex.zw;
		if(reverse)
			vs_out.TexCoords = vec2(1.0 - vs_out.TexCoords.x, vs_out.TexCoords.y);
    }
	
    vs_out.FragPos = fragPos.xyz;
}
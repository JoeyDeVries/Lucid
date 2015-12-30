#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screen;

uniform bool blur;
uniform bool grayscale;
uniform bool invert;

const float offset = 1.0 / 300;  
const vec2 offsets[9] = vec2[](
	vec2(-offset, offset),  // top-left
	vec2(0.0f,    offset),  // top-center
	vec2(offset,  offset),  // top-right
	vec2(-offset, 0.0f),    // center-left
	vec2(0.0f,    0.0f),    // center-center
	vec2(offset,  0.0f),    // center-right
	vec2(-offset, -offset), // bottom-left
	vec2(0.0f,    -offset), // bottom-center
	vec2(offset,  -offset)  // bottom-right    
);

const float kernel[9] = float[](
	1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);

void main()
{
	vec3 color = vec3(0.0f);
	color = texture(screen, TexCoords).rgb;
	if(blur)
	{
		color = vec3(0.0f);
		vec3 sampleTex[9];
		for(int i = 0; i < 9; i++)
		{
			sampleTex[i] = vec3(texture(screen, TexCoords.st + offsets[i]));
		}
		for(int i = 0; i < 9; i++)
			color += sampleTex[i] * kernel[i];
    }
	if(grayscale)
	{
		float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
		color = vec3(average);
	}
	if(invert)
	{
		color = vec3(1.0f) - color;		
	}
    FragColor = vec4(color, 1.0);
    // screenColor *= vec3(1.0, 0.2, 0.2);
    // FragColor = vec4(screenColor, 1.0);
}
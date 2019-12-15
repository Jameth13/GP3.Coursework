#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D color;

void main()
{
	vec3 result = texture(color, TexCoords).rgb;

    float brightness = max(max(result.r, result.g), result.b);
    if(brightness > 1.0)
        BrightColor = vec4(result, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

	FragColor = vec4(result, 1.0);
}

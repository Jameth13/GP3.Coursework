#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;

uniform float exposure;
uniform float gamma;

void main()
{
    vec3 hdrColor = texture(scene, TexCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    vec3 result = hdrColor + bloomColor;
	

	//HDR and gamma
    result = vec3(1.0) - exp(-result * exposure);
    result = pow(result, vec3(1.0 / gamma));

    FragColor = vec4(result, 1.0);
}

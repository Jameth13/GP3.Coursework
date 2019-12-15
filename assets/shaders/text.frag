#version 330 core
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D font;
uniform vec3 textColor;

void main()
{    
    vec4 text = vec4(1.0, 1.0, 1.0, texture(font, TexCoords).r);
    FragColor = vec4(textColor, 1.0) * text;
}

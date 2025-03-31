#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0f);
}

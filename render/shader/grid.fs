#version 330 core
out vec4 FragColor;

uniform float transparency;

void main()
{
    FragColor = vec4(1.0, 1.0, 1.0, transparency); // White with transparency
}
#version 330 core

in vec4 fragColor;
out vec4 FragColor;

void main()
{
    //FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f); // ºÚÉ«
    FragColor = fragColor;
}
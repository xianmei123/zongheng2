#version 330 core

in vec4 outColor;
out vec4 FragColor;

void main()
{
    //FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f); // ��ɫ
    FragColor = outColor;
}
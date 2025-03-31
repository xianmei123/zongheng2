#version 330 core
layout (location = 0) in vec3 aPos;    // 模型原始顶点位置
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aInstancePos; // 实例位置
layout (location = 4) in vec3 aInstanceColor; // 实例颜色
layout (location = 5) in int aInstanceStatus; // 实例状态
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;


out vec3 Color;


void main()
{
    mat4 instanceModel = mat4(1.0);
    instanceModel[3] = vec4(aInstancePos, 1.0); 
    TexCoords = aTexCoords;
    gl_Position = projection * view  * instanceModel * model * vec4(aPos, 1.0);
    Color = aInstanceColor;
    
}
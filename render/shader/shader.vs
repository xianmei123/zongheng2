#version 330 core

layout(location = 0) in vec3 aPos; // Œª÷√ Ù–‘
layout(location = 1) in vec3 aColor;
layout(location = 2) in float camp;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 outColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_PointSize = 7;
    if (camp == 0.0)
	{
		outColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else
	{
		outColor = vec4(0.0, 0.0, 1.0, 1.0);
	}
}
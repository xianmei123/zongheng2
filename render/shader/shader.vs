#version 330 core

layout(location = 0) in vec3 aPos; // Œª÷√ Ù–‘
layout(location = 1) in vec3 aColor;
layout(location = 2) in int status;
layout(location = 3) in int camp;
layout(location = 4) in int padding;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 outColor;
out int camps;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_PointSize = 7;
    if (camp == 0)
	{
		outColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else
	{
		outColor = vec4(0.0, 0.0, 1.0, 1.0);
	}

	if (status == 0) {
		outColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	
	if (padding != 0) {
		outColor = vec4(0.0, 1.0, 0.0, 1.0);
		gl_PointSize = 71;
	}

	camps = camp;
}
#version 330 core
layout (points) in; 
layout (points, max_vertices = 1) out; 

in int camps[];
in vec4 outColor[]; // 从顶点着色器传来的颜色

out vec4 fragColor; // 传递给片段着色器的颜色

void main()
{
    fragColor = outColor[0];
     gl_Position = gl_in[0].gl_Position;
      gl_PointSize = 7;
     EmitVertex();
     
}
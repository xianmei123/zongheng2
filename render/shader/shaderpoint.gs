#version 330 core
layout (points) in; 
layout (points, max_vertices = 1) out; 

in int camps[];
in vec4 outColor[]; // �Ӷ�����ɫ����������ɫ

out vec4 fragColor; // ���ݸ�Ƭ����ɫ������ɫ

void main()
{
    fragColor = outColor[0];
     gl_Position = gl_in[0].gl_Position;
      gl_PointSize = 7;
     EmitVertex();
     
}
#version 330 core
layout (points) in; 
layout (triangle_strip, max_vertices = 6) out; 

in int camps[];
in vec4 outColor[]; // �Ӷ�����ɫ����������ɫ

out vec4 fragColor; // ���ݸ�Ƭ����ɫ������ɫ

void main()
{
    fragColor = outColor[0];
    if (camps[0] == 0) { 
       
      float side = 0.03;  // ���õȱ������εı߳�

    // ����1����������
    vec4 offset1 = vec4(0.0, side * sqrt(3.0) / 2.0, 0.0, 0.0);
    // ����2�����½�
    vec4 offset2 = vec4(-side / 2.0, -side * sqrt(3.0) / 6.0, 0.0, 0.0);
    // ����3�����½�
    vec4 offset3 = vec4(side / 2.0, -side * sqrt(3.0) / 6.0, 0.0, 0.0);

        // ����1
        gl_Position = gl_in[0].gl_Position + offset1;
        EmitVertex();

        // ����2
        gl_Position = gl_in[0].gl_Position + offset2;
        EmitVertex();

        // ����3
        gl_Position = gl_in[0].gl_Position + offset3;
        EmitVertex();

        EndPrimitive(); // ��������λ���
    } else {
        float side = 0.03;  // ���������εı߳�


        // ����1�����Ͻ�
        vec4 offset1 = vec4(-side / 2.0, side / 2.0, 0.0, 0.0);
        // ����2�����Ͻ�
        vec4 offset2 = vec4(side / 2.0, side / 2.0, 0.0, 0.0);
        // ����3�����½�
        vec4 offset3 = vec4(-side / 2.0, -side / 2.0, 0.0, 0.0);
        // ����4�����½�
        vec4 offset4 = vec4(side / 2.0, -side / 2.0, 0.0, 0.0);

        // ʹ������������������������
        // ������1: ����1 (����), ����3 (����), ����2 (����)
        gl_Position = gl_in[0].gl_Position + offset1;
        EmitVertex();
        
        gl_Position = gl_in[0].gl_Position + offset3;
        EmitVertex();
        
        gl_Position = gl_in[0].gl_Position + offset2;
        EmitVertex();
       // EndPrimitive();
        // ������2: ����3 (����), ����4 (����), ����2 (����)
        gl_Position = gl_in[0].gl_Position + offset3;
        EmitVertex();
        
        gl_Position = gl_in[0].gl_Position + offset4;
        EmitVertex();
        
        gl_Position = gl_in[0].gl_Position + offset2;
        EmitVertex();

        EndPrimitive(); // ��������λ���
    }
}
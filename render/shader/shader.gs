#version 330 core
layout (points) in; 
layout (triangle_strip, max_vertices = 6) out; 

in int camps[];
in vec4 outColor[]; // 从顶点着色器传来的颜色

out vec4 fragColor; // 传递给片段着色器的颜色

void main()
{
    fragColor = outColor[0];
    if (camps[0] == 0) { 
       
      float side = 0.03;  // 设置等边三角形的边长

    // 顶点1：顶部正中
    vec4 offset1 = vec4(0.0, side * sqrt(3.0) / 2.0, 0.0, 0.0);
    // 顶点2：左下角
    vec4 offset2 = vec4(-side / 2.0, -side * sqrt(3.0) / 6.0, 0.0, 0.0);
    // 顶点3：右下角
    vec4 offset3 = vec4(side / 2.0, -side * sqrt(3.0) / 6.0, 0.0, 0.0);

        // 顶点1
        gl_Position = gl_in[0].gl_Position + offset1;
        EmitVertex();

        // 顶点2
        gl_Position = gl_in[0].gl_Position + offset2;
        EmitVertex();

        // 顶点3
        gl_Position = gl_in[0].gl_Position + offset3;
        EmitVertex();

        EndPrimitive(); // 完成三角形绘制
    } else {
        float side = 0.03;  // 设置正方形的边长


        // 顶点1：左上角
        vec4 offset1 = vec4(-side / 2.0, side / 2.0, 0.0, 0.0);
        // 顶点2：右上角
        vec4 offset2 = vec4(side / 2.0, side / 2.0, 0.0, 0.0);
        // 顶点3：左下角
        vec4 offset3 = vec4(-side / 2.0, -side / 2.0, 0.0, 0.0);
        // 顶点4：右下角
        vec4 offset4 = vec4(side / 2.0, -side / 2.0, 0.0, 0.0);

        // 使用两个三角形来绘制正方形
        // 三角形1: 顶点1 (左上), 顶点3 (左下), 顶点2 (右上)
        gl_Position = gl_in[0].gl_Position + offset1;
        EmitVertex();
        
        gl_Position = gl_in[0].gl_Position + offset3;
        EmitVertex();
        
        gl_Position = gl_in[0].gl_Position + offset2;
        EmitVertex();
       // EndPrimitive();
        // 三角形2: 顶点3 (左下), 顶点4 (右下), 顶点2 (右上)
        gl_Position = gl_in[0].gl_Position + offset3;
        EmitVertex();
        
        gl_Position = gl_in[0].gl_Position + offset4;
        EmitVertex();
        
        gl_Position = gl_in[0].gl_Position + offset2;
        EmitVertex();

        EndPrimitive(); // 完成正方形绘制
    }
}
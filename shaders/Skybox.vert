#version 330 core
layout (location = 0) in vec3 in_Vertex;

out vec3 TexCoords;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    TexCoords = in_Vertex;
    vec4 pos = u_Projection * u_View * vec4(in_Vertex, 1.0);
    gl_Position = pos.xyww;
}

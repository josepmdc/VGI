#version 330 core

layout(location = 0) in vec3 in_Positions;
layout(location = 1) in vec2 in_TexCoordinates;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec2 texCoordinates;

void main() {
    gl_Position = u_Projection * u_View * u_Model * vec4(in_Positions, 1.0);
    texCoordinates = in_TexCoordinates;
}


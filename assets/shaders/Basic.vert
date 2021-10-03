#version 330 core

layout(location = 0) in vec4 in_Positions;

uniform mat4 u_Transform;

void main() {
    gl_Position = u_Transform * in_Positions;
}

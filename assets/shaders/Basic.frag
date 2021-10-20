#version 330 core

in vec2 texCoord;

out vec4 color;

uniform vec4 u_Color;
// uniform sampler2D u_Texture;

void main() {
    color = u_Color;
    // color = texture(u_Texture, texCoord);
}

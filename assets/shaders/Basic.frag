#version 330 core
out vec4 FragColor;

in vec2 in_TexCoords;

uniform sampler2D u_TextureDiffuse;

void main() {    
    FragColor = texture(u_TextureDiffuse, in_TexCoords);
}

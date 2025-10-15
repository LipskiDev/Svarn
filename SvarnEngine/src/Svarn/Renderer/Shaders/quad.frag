#version 330 core

in vec2 v_UV;
out vec4 fragColor;

uniform sampler2D u_Tex;

void main() {
    fragColor = texture(u_Tex, v_UV);
}

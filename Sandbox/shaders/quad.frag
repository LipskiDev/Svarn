#version 330 core

in vec2 v_UV;
out vec4 fragColor;

uniform sampler2D u_Tex;

void main() {
    float d = texture(u_Tex, v_UV).r;
    fragColor = vec4(d, d, d, 1.0);
}

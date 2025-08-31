#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_TexCoord;

out vec2 v_UV;

void main() {
    v_UV = a_TexCoord;
    gl_Position = vec4(a_Position, 1.0);
}

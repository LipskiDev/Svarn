#version 420 core

layout(location = 0) out vec4 color;
uniform sampler2D texture0;

in vec3 v_Position;
in vec2 v_TexCoord;

void main()
{
    color = texture(texture0, v_TexCoord); 
}

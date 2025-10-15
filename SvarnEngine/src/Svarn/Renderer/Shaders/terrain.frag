#version 420 core

layout(location = 0) out vec4 outcolor;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec4 v_Tangent;
in mat3 v_TangentBasis;


void main()
{
    outcolor = vec4(1.0, 1.0, 1.0, 1.0);
}

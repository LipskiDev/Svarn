#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec4 a_Tangent;
layout(location = 4) in vec4 a_Bitangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 VP;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoord;
out vec4 v_Tangent;
out mat3 v_TangentBasis;


void main()
{
    // world-space position
    vec4 worldPos = modelMatrix * vec4(a_Position, 1.0);
    v_Position = worldPos.xyz;

    v_TangentBasis = mat3(modelMatrix) * mat3(a_Tangent, a_Bitangent, a_Normal);

    // transform normal
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    v_Normal = normalize(normalMatrix * a_Normal);

    // tangent (xyz) transformed like a direction
    vec3 T = normalize(mat3(modelMatrix) * a_Tangent.xyz);
    v_Tangent = vec4(T, a_Tangent.w); // keep handedness in .w

    // uv
    v_TexCoord = a_TexCoord;

    // final position
    gl_Position = VP * worldPos;
}

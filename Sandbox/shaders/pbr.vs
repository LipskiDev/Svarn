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

mat3 computeNormalMatrix(mat4 M) {
    return transpose(inverse(mat3(M)));
}

void main()
{
    // world-space position
    vec4 worldPos = modelMatrix * vec4(a_Position, 1.0);
    v_Position = worldPos.xyz;

    mat3 Nmat = computeNormalMatrix(modelMatrix);
    vec3 N = normalize(Nmat * a_Normal);

    vec3 Traw = mat3(modelMatrix) * a_Tangent.xyz;

    vec3 T = normalize(Traw - N * dot(Traw, N));

    float handedness = a_Tangent.w;
    vec3 B = cross(N, T) * handedness;
    
    v_Normal = N;
    v_Tangent = vec4(T, handedness);
    v_TexCoord = a_TexCoord;
    v_TangentBasis = mat3(T, B, N);

    gl_Position = VP * worldPos;
}

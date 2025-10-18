#version 420 core

layout(location = 0) out vec4 outcolor;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec4 v_Tangent;
in mat3 v_TangentBasis;

float remapHeight(float h) {
    float minH = -50.0;
    float maxH = -20.0;
    return clamp((h - minH) / (maxH - minH), 0.0, 1.0);
}

#define HEIGHT_SCALE = 20
void main()
{
    float h = remapHeight(v_Position.y - 40.0);
    vec3 color;

    if (h <= 0.2) {
        // Deep water
        color = mix(vec3(0.0, 0.0, 0.3), vec3(0.0, 0.2, 0.6), h / 0.2);
    } else if (h <= 0.3) {
        // Beach
        color = mix(vec3(0.76, 0.70, 0.50), vec3(0.85, 0.80, 0.60), (h - 0.2) / 0.1);
    } else if (h <= 0.6) {
        // Grass
        color = mix(vec3(0.0, 0.4, 0.0), vec3(0.2, 0.6, 0.2), (h - 0.3) / 0.3);
    } else if (h <= 0.8) {
        // Rock
        color = mix(vec3(0.3, 0.3, 0.3), vec3(0.6, 0.6, 0.6), (h - 0.6) / 0.2);
    } else {
        // Snow
        color = vec3(1.0, 1.0, 1.0);
    }

    outcolor = vec4(color, 1.0);
}

#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec4 a_Tangent;
layout(location = 4) in vec4 a_Bitangent;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoord;
out vec4 v_Tangent;
out mat3 v_TangentBasis;

// Permutation table duplicated to 512 entries (avoids extra masking)
const int PERM_SIZE = 512;
const int perm[PERM_SIZE] = int[PERM_SIZE](
        // 0..255
        151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
        8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 35, 11,
        88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
        77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
        102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
        135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
        5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
        223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
        129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
        251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
        49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
        138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180, 117,
        32, 57, 177, 33, 203,
        // 256..511 (repeat)
        151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
        8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 35, 11,
        88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
        77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
        102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
        135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
        5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
        223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
        129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
        251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
        49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
        138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180, 117,
        32, 57, 177, 33, 203
    );

// -----------------------------------------------------------------------------
// Helpers
float Lerp(float t, float a, float b) {
    return a + t * (b - a);
}

float Fade(float t) {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

float Grad(int hash, float x, float y, float z) {
    int h = hash & 15;
    float u = (h < 8) ? x : y;
    float v = (h < 4) ? y : ((h == 12 || h == 14) ? x : z);
    float r1 = ((h & 1) == 0) ? u : -u;
    float r2 = ((h & 2) == 0) ? v : -v;
    return r1 + r2;
}

// -----------------------------------------------------------------------------
// Perlin noise (2D input; uses z = 0.5 slice of 3D noise)
float PerlinNoise(vec2 uv) {
    float x = uv.x;
    float y = uv.y;
    float z = 0.5;

    int X = int(floor(x)) & 255;
    int Y = int(floor(y)) & 255;
    int Z = int(floor(z)) & 255;

    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    float u = Fade(x);
    float v = Fade(y);
    float w = Fade(z);

    int A = perm[X] + Y;
    int AA = perm[A] + Z;
    int AB = perm[A + 1] + Z;
    int B = perm[X + 1] + Y;
    int BA = perm[B] + Z;
    int BB = perm[B + 1] + Z;

    float res = Lerp(
            w,
            Lerp(
                v,
                Lerp(u, Grad(perm[AA], x, y, z),
                    Grad(perm[BA], x - 1., y, z)),
                Lerp(u, Grad(perm[AB], x, y - 1., z),
                    Grad(perm[BB], x - 1., y - 1., z))
            ),
            Lerp(
                v,
                Lerp(u, Grad(perm[AA + 1], x, y, z - 1.),
                    Grad(perm[BA + 1], x - 1., y, z - 1.)),
                Lerp(u, Grad(perm[AB + 1], x, y - 1., z - 1.),
                    Grad(perm[BB + 1], x - 1., y - 1., z - 1.))
            )
        );

    return res;
}

#define NUM_OCTAVES 8
#define PERSISTENCE 0.75
#define LACUNARITY 0.75

float rand(vec2 n) {
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float noise(vec2 p) {
    float amplitude = 1.0f;
    float frequency = 1.0f;

    float height = 0.0f;

    for (int i = 0; i < NUM_OCTAVES; i++) {
        float x_sample = p.x * frequency / 10.0;
        float y_sample = p.y * frequency / 10.0;

        float perlin_value = PerlinNoise(vec2(x_sample, y_sample));
        height += perlin_value * amplitude;

        amplitude *= PERSISTENCE;
        frequency *= LACUNARITY;
    }

    return height;
}

mat3 computeNormalMatrix(mat4 M) {
    return transpose(inverse(mat3(M)));
}

vec3 noiseNormal(vec2 p, float eps) {
    float dx = (noise(p + vec2(eps, 0.0)) - noise(p - vec2(eps, 0.0)));
    float dy = (noise(p + vec2(0.0, eps)) - noise(p - vec2(0.0, eps)));
    return vec3(dx, 0.0, dy);
}

void main()
{
    // world-space position
    vec4 worldPos = u_ModelMatrix * vec4(a_Position, 1.0);
    worldPos.y += noise(worldPos.xz) * 20;

    v_Position = worldPos.xyz;

    mat3 Nmat = computeNormalMatrix(u_ModelMatrix);
    vec3 N = normalize(Nmat * a_Normal);

    vec3 Traw = mat3(u_ModelMatrix) * a_Tangent.xyz;

    vec3 T = normalize(Traw - N * dot(Traw, N));

    float handedness = a_Tangent.w;
    vec3 B = cross(N, T) * handedness;

    float epsilon = 1.0;

    v_Normal = noiseNormal(worldPos.xz, epsilon);
    v_Tangent = vec4(T, handedness);
    v_TexCoord = a_TexCoord;
    v_TangentBasis = mat3(T, B, N);

    gl_Position = u_ProjectionMatrix * u_ViewMatrix * worldPos;
}

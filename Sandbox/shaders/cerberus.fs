#version 420 core

layout(location = 0) out vec4 outcolor;

uniform sampler2D texture0; // albedo
uniform sampler2D texture1; // normal
uniform sampler2D texture2; // roughness
uniform sampler2D texture3; // metallic

uniform vec3 u_CameraPosition;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_TexCoord;
in vec4 v_Tangent;
in mat3 v_TangentBasis;


const float Epsilon = 0.00001;

struct DirLight { vec3 direction; vec3 radiance; };

uniform DirLight u_DirLight;

const float PI = 3.14159265359;

vec3 srgbToLinear(vec3 srgbColor) {
    vec3 cutoff = vec3(0.04045);
    vec3 low  = srgbColor / 12.92;
    vec3 high = pow((srgbColor + 0.055) / 1.055, vec3(2.4));
    return mix(low, high, step(cutoff, srgbColor));
}

vec3 getWorldNormal()
{
    vec3 N = normalize(v_Normal);
    vec3 T = normalize(v_Tangent.xyz);
    vec3 B = normalize(cross(N, T) * v_Tangent.w);
    mat3 TBN = mat3(T, B, N);

    vec3 nTS = texture(texture3, v_TexCoord).xyz * 2.0 - 1.0;
    return normalize(TBN * nTS);
}

float ndfGGX(float cosLh, float roughness)
{
	float alpha   = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}

float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;
	return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
	return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{

    vec3 albedo = srgbToLinear(texture(texture0, v_TexCoord).rgb);
    float roughness = clamp(texture(texture2, v_TexCoord).r, 0.04, 1.00);
    float metallic = clamp(texture(texture3, v_TexCoord).r, 0.04, 1.00);

    vec3 Lo = normalize(u_CameraPosition - v_Position);

    vec3 N = normalize(getWorldNormal());
    N = normalize(v_TangentBasis * N);

    float cosLo = max(0.0, dot(N, Lo));

    vec3 Lr = 2.0 * cosLo * N - Lo;

    vec3 F0 = mix(vec3(0.04f), albedo, metallic);

    vec3 Li = -u_DirLight.direction;
    vec3 Lradiance = u_DirLight.radiance;

    vec3 Lh = normalize(Li + Lo);

    float cosLi = max(0.0, dot(N, Li));
    float cosLh = max(0.0, dot(N, Lh));

    vec3 F = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));

    float D = ndfGGX(cosLh, roughness);

    float G = gaSchlickGGX(cosLi, cosLo, roughness);

    vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metallic);

    vec3 diffuseBRDF = kd * albedo;

    vec3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosLi * cosLo);

    vec3 directLighting = (diffuseBRDF + specularBRDF) * Lradiance * cosLi;

    outcolor = vec4(albedo, 1.0);
    // outcolor = vec4(diffuseBRDF, 1.0);
}

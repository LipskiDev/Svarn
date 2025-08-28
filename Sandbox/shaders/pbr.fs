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

vec3 fresnelSchlick(float cosTheta, vec3 F0) 
{
    return F0 + (1 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

void main()
{
    vec3 albedo = texture(texture0, v_TexCoord).rgb;
    vec3 normal = texture(texture1, v_TexCoord).rgb;
    float roughness = texture(texture2, v_TexCoord).r;
    float metallic = texture(texture3, v_TexCoord).r;

    vec3 N = normalize(normal);
    vec3 V = normalize(u_CameraPosition - v_Position);

    vec3 Lo = vec3(0.0);

    vec3 L = u_DirLight.direction;
    vec3 H = normalize(V + L);

    vec3 radiance = u_DirLight.radiance;

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;

    kD *= 1.0 - metallic;

    float NdotL = max(dot(N, L), 0.0);
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;

    vec3 ambient = vec3(0.03) * albedo;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    outcolor = vec4(color, 1.0);

}

#version 420 core

layout(location = 0) out vec4 outcolor;

struct Material {
    bool useAlbedoTexture;
    bool useNormalTexture;
    bool useRoughnessTexture;
    bool useMetallicTexture;

    sampler2D albedoTexture; // albedo
    sampler2D normalTexture; // normal
    sampler2D roughnessTexture; // roughness
    sampler2D metallicTexture; // metallic
    
    vec3 albedo;
    float roughness;
    float metallic;
};

uniform Material material;

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
   // Fetch material parameters
    vec3  albedo = material.useAlbedoTexture
        ? texture(material.albedoTexture, v_TexCoord).rgb   // assume sRGB decode is enabled on the sampler/texture
        : material.albedo;
    float roughness = material.useRoughnessTexture
        ? texture(material.roughnessTexture, v_TexCoord).r
        : material.roughness;
    float metallic = material.useMetallicTexture
        ? texture(material.metallicTexture, v_TexCoord).r
        : material.metallic;    vec3 normal;


    roughness = clamp(roughness, 0.0, 1.0);
    metallic = clamp(metallic, 0.0, 1.0);

    vec3 T = normalize(v_TangentBasis[0]);
    vec3 B = normalize(v_TangentBasis[1]) * sign(v_Tangent.w);
    vec3 Ngeom = normalize(v_TangentBasis[2]);
    mat3 TBN = mat3(T, B, Ngeom);
    
    vec3 N;
    if (material.useNormalTexture) {
        vec3 n_ts = texture(material.normalTexture, v_TexCoord).xyz * 2.0 - 1.0; // tangent space
        // Optionally renormalize XY for BC5/AG normals: n_ts.z = sqrt(max(1.0 - dot(n_ts.xy, n_ts.xy), 0.0));
        N = normalize(TBN * n_ts);  // to world space
    } else {
        N = Ngeom;
    }

    vec3 V = normalize(u_CameraPosition - v_Position);

    vec3 L = normalize(-u_DirLight.direction);
    vec3 H = normalize(V + L);

    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);

    vec3 numerator = NDF * G * F;
    float denom = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 1e-4;
    vec3 specular = numerator / denom;

    vec3 kS = F;
    vec3 kD = (1.0 - kS) * (1.0 - metallic);

    float NdotL = max(dot(N, L), 0.0);
    vec3 Lo = (kD * albedo / PI + specular) * u_DirLight.radiance * NdotL;

    vec3 ambient = vec3(0.03) * albedo;

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/ 2.2));

    outcolor = vec4(color, 1.0);

}

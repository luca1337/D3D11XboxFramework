
#define MAX_LIGHTS 10

//for texturing
Texture2D albedo_texture : register(t0);
Texture2D normal_texture : register(t1);
Texture2D specular_texture : register(t2);

SamplerState sampler_state0 : register(s0);

struct LightingResult
{
    float4 Diffuse;
    float4 Specular;
    float4 NormalMapping;
    float intensity;
};

struct Material
{
    float4 Emissive;
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float SpecularPower;
    int UseTexture;
    int UseNormal;
};

struct Light
{
    float3 position;
    float range;
    float3 direction;
    float3 attenuation;
    float4 color;
    int type;
    int is_active;
    float intensity;
};

cbuffer clights : register(b1)
{
    Light lights[MAX_LIGHTS];
};

cbuffer clight_properties : register(b2) // must exists only once!
{
    float4 global_ambient;
    float3 eye_position;
}

cbuffer cMaterial : register(b3)
{
    Material material;
};

struct ps_in
{
    float4 out_position     : SV_Position;
    float3 world_position   : VERTEXPOS;
    float3 world_normal     : OUTNORMAL;
    float2 world_uv         : OUTUV;
    float3 world_tangent    : OUTTANGENT;
    float3 world_bitangent  : OUTBITANGENT;
};

struct ps_out
{
    float4 pixel_color      : SV_Target;
};

float4 CalculateDiffuse(float4 color, float3 L, float3 N)
{
    float NdotL = max(0, dot(N, L));
    return color * NdotL;
}

float4 CalculateSpecular(float4 color, float3 V, float3 L, float3 N)
{
    float3 R = normalize(reflect(-L, N));
    float RdotV = max(0, dot(R, V));

    float3 H = normalize(L + V);
    float NdotH = max(0, dot(N, H));

    return color * pow(RdotV, material.SpecularPower);
}

float CalculateAttenuation(float x, float y, float z, float d)
{
    return 1.0f / (x + y * d + z * d * d);
}

float3 CalculateNormalMapping(ps_in IN)
{
    // get the normal map sampler color
    float4 normalColor = normal_texture.Sample(sampler_state0, IN.world_uv);

    // uncompress each value from [0, 1] to [-1, 1]
    normalColor = (normalColor * 2.0f) - 1.0f;

    float3 normalBump = (normalColor.x * IN.world_tangent) + (normalColor.y * IN.world_bitangent) + (normalColor.z * normalize(IN.world_normal));

    normalBump = normalize(normalBump);

    return normalBump;
}

LightingResult CalculateDirectionalLight(ps_in IN, int index)
{
    LightingResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, 0 };

    float3 L = normalize(-lights[index].direction);

    result.Diffuse = CalculateDiffuse(lights[index].color, L, normalize(IN.world_normal));
    result.Specular = CalculateSpecular(lights[index].color, normalize(eye_position - IN.world_position), L, normalize(IN.world_normal));

    if (material.UseNormal)
    {
        float3 normal_sample = CalculateNormalMapping(IN);

        float light_intensity = saturate(dot(normal_sample, L));

        result.NormalMapping = saturate(result.Diffuse * light_intensity);
    }
    else
    {
        result.NormalMapping = float4(1, 1, 1, 1);
    }

    return result;
}

LightingResult CalculatePointLight(ps_in IN, int index)
{
    LightingResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, 0 };

    float3 input_normal = normalize(IN.world_normal);

    float3 L = (lights[index].position - IN.world_position);

    float distance = length(L);

    L = L / distance * lights[index].range;

    float attenuation = CalculateAttenuation(lights[index].attenuation[0], lights[index].attenuation[1], lights[index].attenuation[2], distance);

    result.Diffuse = CalculateDiffuse(lights[index].color, L, input_normal) * attenuation;
    result.Specular = CalculateSpecular(lights[index].color, normalize(eye_position - IN.world_position), L, input_normal) * attenuation;

    if (material.UseNormal)
    {
        float3 normal_sample = CalculateNormalMapping(IN);

        float light_intensity = saturate(dot(normal_sample, L));

        result.NormalMapping = saturate(result.Diffuse * light_intensity);
    }
    else
    {
        result.NormalMapping = float4(1, 1, 1, 1);

    }

    return result;
}

LightingResult ComputeLights(ps_in IN)
{
    LightingResult totalResult = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, 0 };

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        LightingResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, 0 };

        if (!lights[i].is_active)
            continue;

        switch (lights[i].type)
        {
            case 0:
                result = CalculateDirectionalLight(IN, i);
                break;
            case 1:
                result = CalculatePointLight(IN, i);
                break;
        }

        totalResult.Diffuse += saturate(result.Diffuse) * lights[i].intensity;
        totalResult.Specular += saturate(result.Specular) * lights[i].intensity;
        totalResult.NormalMapping += result.NormalMapping;
    }

    totalResult.Diffuse = totalResult.Diffuse;
    totalResult.Specular = totalResult.Specular;
    totalResult.NormalMapping = saturate(totalResult.NormalMapping);

    return totalResult;
}

ps_out main(ps_in IN)
{
    ps_out OUT;
    OUT.pixel_color = float4(0, 0, 0, 0);

    IN.world_uv *= 6;

    LightingResult result = ComputeLights(IN);

    float4 emissive = material.Emissive;
    float4 ambient = material.Ambient * global_ambient;
    float4 diffuse = material.Diffuse * result.Diffuse;
    float4 specular = material.Specular * result.Specular;

    float4 albedo_color = { 1, 1, 1, 1 };

    if (material.UseTexture)
    {
        albedo_color = albedo_texture.Sample(sampler_state0, IN.world_uv);
    }

    OUT.pixel_color = (emissive + ambient + diffuse + specular) * albedo_color * result.NormalMapping;
    
    return OUT;
}
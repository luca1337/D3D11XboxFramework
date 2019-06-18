
cbuffer matrices : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};

struct vs_in
{
    float3 in_position		: VERTICES;
    float3 in_normal		: NORMALS;
	float2 in_uv			: UVS;
    float3 in_tangent       : TANGENTS;
    float3 in_bitangent     : BITANGENTS;
};

struct vs_out
{
    float4 out_position		: SV_Position;
	float3 world_position	: VERTEXPOS;
	float3 world_normal		: OUTNORMAL;
	float2 world_uv			: OUTUV;
    float3 world_tangent    : OUTTANGENT;
    float3 world_bitangent  : OUTBITANGENT;
};

vs_out main(vs_in IN)
{
    vs_out OUT;

	OUT.world_position = mul(model, float4(IN.in_position, 1)).xyz;
	OUT.world_normal = mul(model, float4(IN.in_normal, 0));
    OUT.world_tangent = mul(model, float4(IN.in_tangent, 0));
    OUT.world_tangent = normalize(OUT.world_tangent);
    OUT.world_bitangent = mul(model, float4(IN.in_bitangent, 0));
    OUT.world_bitangent = normalize(OUT.world_bitangent);
	OUT.out_position = mul(projection, mul(view, float4(OUT.world_position, 1)));

	OUT.world_uv = IN.in_uv;

    return OUT;
}
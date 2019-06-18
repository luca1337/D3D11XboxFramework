
cbuffer matrices
{
	matrix model;
	matrix view;
	matrix projection;
};

struct vs_skymap_in
{
	float4 in_position		: VERTICES;
};

struct skymap_vs_out
{
	float4 out_position		: SV_Position;
	float4 dome_position	: DOME;
};

skymap_vs_out main(vs_skymap_in IN)
{
	skymap_vs_out OUT;

	IN.in_position.w = 1.0;

	OUT.out_position = mul(model, IN.in_position);
	OUT.out_position = mul(view, OUT.out_position);
	OUT.out_position = mul(projection, OUT.out_position);

	OUT.dome_position = IN.in_position;

	return OUT;
}
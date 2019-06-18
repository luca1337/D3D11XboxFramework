
struct ps_skymap_in
{
	float4 out_position		: SV_Position;
	float4 dome_position	: DOME;
};

struct ps_skymap_out
{
	float4 pixel_color : SV_Target;
};

ps_skymap_out main(ps_skymap_in IN)
{
	ps_skymap_out OUT;

	float height;

	height = IN.dome_position.y;

	if (height < 0.0)
	{
		height = 0.0;
	}

	OUT.pixel_color = lerp(float4(0.82f, 0.85f, 0.90, 1.0f), float4(0.25f, 0.52f, 0.95f, 1.0f), height);

	return OUT;
}
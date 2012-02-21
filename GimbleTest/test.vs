
cbuffer CbMats : register(b0)
{
	matrix World : packoffset(c0);
	matrix VP : packoffset(c4);
};

struct VS_INPUT
{
	float4 position	: POSITION;
	float3 normal : NORMAL;
	float2 texcoord	: TEXCOORD0;
};

struct VS_OUTPUT
{
	float3 normal : NORMAL;
	float2 texcoord	: TEXCOORD0;
	float4 position	: SV_POSITION;
};

VS_OUTPUT V(VS_INPUT Input)
{
	VS_OUTPUT Output;
	
	Output.position = mul(Input.position, mul(World, VP));
	Output.normal = mul(Input.normal, (float3x3)World);
	Output.texcoord = Input.texcoord;
	
	return Output;
}

gimble:Vertex vs_4_0 V

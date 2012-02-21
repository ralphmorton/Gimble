
matrix W;
matrix WS;
matrix VP;

float3 LightDir = float3(-1,-1,-1);
float3 CamDir;
float Time;

SamplerState TexSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

Texture2D Tex;

struct v_in
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD;
};

struct p_in
{
    float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 tc : TEXCOORD0;
};

struct v_in2
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
};

struct p_in2
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
};

p_in V(v_in input)
{
    p_in output = (p_in)0;

    output.pos = mul(input.pos, mul(W,VP));
	output.normal = normalize(mul(input.normal, W));
	output.tc = input.texcoord;

    return output;
}

float4 P(p_in input) : SV_Target
{
	float camdot = dot(input.normal, -normalize(CamDir));
	float mul = step(camdot, 0.35f) * 10;

	clip(mul-0.1f);

    return float4(0,0,0,1);
}

p_in2 V2(v_in2 input)
{
    p_in2 output = (p_in2)0;
    output.pos = mul(input.pos, mul(W,VP));
	output.normal = normalize(mul(input.normal, W));
    return output;
}

float4 P2(p_in2 input) : SV_Target
{
	float camdot = dot(input.normal, -normalize(CamDir));
	return float4(1,0,0,1) * camdot;
}

technique10 Render
{
	pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, V2() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, P2() ) );
    }
}


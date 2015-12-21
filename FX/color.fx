//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject
{
	float4x4 gWorldViewProj; 
};

SamplerState samAnisotropic
{
	//Filter = ANISOTROPIC;
	////MaxAnisotropy = 4;

	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};
Texture2D gDiffuseMap;

struct VertexIn
{
	float3 Pos   : POSITION;
	//float3 Nor :	NORMAL;
 //   float4 Color : COLOR;
	float2 UV : TEXCOORD0;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    //float4 Color : COLOR;
	float2 UV : TEXCOORD0;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
	
	// Just pass vertex color into the pixel shader.
    //vout.Color = vin.Color;
	vout.UV = vin.UV;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
	return gDiffuseMap.Sample(samAnisotropic, pin.UV);
}

technique11 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}


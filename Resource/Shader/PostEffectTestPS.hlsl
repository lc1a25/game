#include "PostEffectTest.hlsli"

Texture2D<float4> tex : register(t0);//0番目に設定されたテクスチャ
SamplerState smp : register(s0);//0番目に設定されたサンプラー



float4 main(VSOutput input) : SV_TARGET
{
	float u = 1.0f / 1280.0f;
	float v = 1.0f / 720.0f;
	float4 total = { 0.0f,0.0f,0.0f,0.0f };

	int width = 10;
	int height = 10;
	for (int i = 0; i < height; i++)
	{	
		//total = tex.Sample(smp, input.uv + float2(u, v));
	
		for (int j = 0; j < width; j++)
		{
			total +=  tex.Sample(smp, input.uv + float2(u * j, v * i));
	
		}
	}

	total = total / (width * height);
	return float4(total.xyz, 1);
	//float4 texcolor = tex.Sample(smp,input.uv + float2(0.5,0));
	//float2 limit = { 1,1,1 };
	//return float4( texcolor.rgb, 1);
	//return float4 (1,1,1,1);
}


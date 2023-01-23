//#include "BasicShaderHeader.hlsli"
#include "Particle.hlsli"

static const uint vnum = 4;

static const float4 offset_array[vnum] =
{
	float4(-0.5f,-0.5f,0,0),
	float4(-0.5f,+0.5f,0,0),
	float4(+0.5f,-0.5f,0,0),
	float4(+0.5f,+0.5f,0,0)
};

static const float2 uv_array[vnum] =
{
	float2(0,1),
	float2(0,0),
	float2(1,1),
	float2(1,0)
};

[maxvertexcount(vnum)]
void main(
	//triangle VSOutput input[3] : SV_POSITION,
	point VSOutput input[1] : SV_POSITION,

	//inout LineStream< GSOutput > output
	inout TriangleStream< GSOutput > output
	//inout PointStream< GSOutput > output
)
{

	GSOutput element;

	for (uint i = 0; i < vnum; i++)
	{
		//float4 offset = mul(matBillboard, offset_array[i]);
		//中心からのオフセットをスケーリング
		float4 offset = offset_array[i] * input[0].scale ;
		//中心からのオフセットをビルボード回転(モデル回転)
		offset = mul(matBillboard, offset);
		element.svpos = input[0].pos + offset;
		
		element.svcolor = input[0].color;

		element.svpos = mul(mat, element.svpos);
		//element.uv =  float2(0.5f, 0.5f);
		element.uv = uv_array[i];
		output.Append(element);
	}


}
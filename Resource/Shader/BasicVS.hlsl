#include "BasicShaderHeader.hlsli"


VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;
	/*output.svpos = pos;*/
	output.svpos = mul(mat, pos);//���W�ɍs�����Z
	output.normal = normal;//�󂯎�����@�������̂܂܏o��
	output.uv = uv;
	return output;
}
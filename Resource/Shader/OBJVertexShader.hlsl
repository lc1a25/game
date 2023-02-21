#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD , float4 color : COLOR)
{
	float4 wnormal = normalize(mul(world,float4(normal,0)));
	float4 wpos = mul(world, pos);
	float3 lightdir = normalize(float3(0, -1, 1));
	float3 lightcolor = float3(1, 1, 1);
	float3 ambient = m_ambient;

	float3 diffuse = dot(-lightdir, wnormal.xyz) * m_diffuse;

	//const float3 eye = float3(0, 0, -20);
	//float3 eyedir = normalize(eye - pos.xyz);
	float3 eyedir = normalize(cameraPos - wpos.xyz);

	const float shininess = 4.0f;

	float3 reflect = normalize(lightdir + 2 * dot(-lightdir, wnormal.xyz) * wnormal.xyz);

	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	VSOutput output; // ピクセルシェーダーに渡す値
	output.color.rgb = (ambient + diffuse + specular );
	output.color.a = m_alpha;
	output.svpos = mul(mul(viewProj,world), pos);
	output.normal = normal;
	output.uv = uv;
	return output;
}
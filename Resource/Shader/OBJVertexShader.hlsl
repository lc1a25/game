#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD , float4 color : COLOR)
{
	float3 lightdir = normalize(float3(0, -1, 1));
	float3 lightcolor = float3(1, 1, 1);
	float3 ambient = m_ambient;
	float3 diffuse = dot(-lightdir, normal) * m_diffuse;

	const float3 eye = float3(0, 0, -20);

	const float shininess = 4.0f;

	float3 eyedir = normalize(eye - pos.xyz);

	float3 reflect = normalize(lightdir + 2 * dot(-lightdir, normal) * normal);

	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	VSOutput output; // ピクセルシェーダーに渡す値
	output.color.rgb = (ambient + diffuse + specular) * lightcolor;
	output.color.a = m_alpha;
	output.svpos = mul(mat, pos);
	output.normal = normal;
	output.uv = uv;
	return output;
}
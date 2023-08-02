#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	// 法線にワールド行列によるスケーリング・回転を適用
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	VSOutput output; // ピクセルシェーダーに渡す値
	output.svpos = mul(mul(viewproj, world), pos);

	output.worldpos = mul(world, pos);
	output.normal = wnormal.xyz;
	output.uv = uv;

	return output;
	//float4 wnormal = normalize(mul(world,float4(normal,0)));
	//float4 wpos = mul(world, pos);
	////float3 lightdir = normalize(float3(1, -1.5, 1));
	////float3 lightcolor = float3(1, 1, 1);
	//float3 ambient = m_ambient;

	//float3 lightv1 = normalize(lightv);
	////float3 diffuse = dot(-lightdir, wnormal.xyz) * m_diffuse;
	//float3 diffuse = dot(-lightv1, wnormal.xyz) * m_diffuse;

	//float3 eyedir = normalize(cameraPos - wpos.xyz);

	//const float shininess = 1.0f;

	////float3 reflect = normalize(lightdir + 2 * dot(-lightdir, wnormal.xyz) * wnormal.xyz);
	//float3 reflect = normalize(lightv1 + 2 * dot(-lightv1 , wnormal.xyz) * wnormal.xyz);


	//float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	//VSOutput output; // ピクセルシェーダーに渡す値
	//output.color.rgb = (ambient + diffuse + specular );
	//output.color.a = m_alpha;
	//output.svpos = mul(mul(viewProj,world), pos);
	//output.normal = normal;
	//output.uv = uv;
	//return output;
}
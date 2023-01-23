cbuffer cbuff0 : register(b0)
{
	//float4 color; // 色(RGBA)
	matrix mat; // ３Ｄ変換行列
	matrix matBillboard;
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 pos : POSITION; // 頂点座標
	//float4 svpos : SV_POSITION; // システム用頂点座標
	//float3 normal :NORMAL; // 法線ベクトル
	//float2 cv :UV; // uv値
	float scale : TEXCOORD;//スケール
	float4 color : COLOR; // 色(RGBA)
};

//ジオメトリシェーダからピクセルシェーダへの出力
struct GSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	//float3 normal :NORMAL; // 法線ベクトル
	float2 uv  :TEXCOORD; // uv値
	float4 svcolor : svCOLOR; // 色(RGBA)
};

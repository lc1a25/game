cbuffer cbuff0 : register(b0)
{
	//matrix mat; // ３Ｄ変換行列
	matrix viewproj;//ビュープロジェクション
	matrix world;//world行列
	float3 cameraPos;//カメラの座標
};

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient : packoffset(c0);//アンビエント係数
	float3 m_diffuse : packoffset(c1);//ディフューズ係数
	float3 m_specular : packoffset(c2);//スペキュラー係数
	float m_alpha : packoffset(c2.w);//アルファ
};

//平行光源の数
static const int DIRLIGHT_NUM = 3;
static const int SHADOWCIRCLE_NUM = 1;

struct DirLight
{
	float3 lightv;    // ライトへの方向の単位ベクトル
	float3 lightcolor;    // ライトの色(RGB)
	uint active;
}; 
struct ShadowCircle
{
	float3 dir;//投影方向の逆ベクトル(単位ベクトル)
	float3 casterPos;//キャスター座標
	float distanceCasterLight;//キャスターとライトの距離
	float3 atten;//距離減衰係数
	float2 factorAngleCos;//減衰角度のコサイン
	uint active;
};

cbuffer cbuff2 : register(b2)
{
	float3 ambientColor;
	DirLight dirLights[DIRLIGHT_NUM];
	ShadowCircle shadowCircles[SHADOWCIRCLE_NUM];
	//float3 lightv;    //ライトへの方向の単位ベクトル
	//float3 lightcolor;//ライトの色rgb
}

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	
	float4 worldpos : POSITION; // ワールド座標
	float3 normal :NORMAL; // 法線
	float2 uv  :TEXCOORD; // uv値
	//float3 normal :NORMAL; // 法線ベクトル
	//float2 uv  :TEXCOORD; // uv値
	//float4 color : COLOR;
};

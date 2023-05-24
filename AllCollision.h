
#pragma once
#include <DirectXMath.h>
class AllCollision
{
private:
	//“–‚½‚è”»’è—p•Ï”
	float length = 0.0f;
	float size = 25.0f;
	float wallColliLength = 0.0f;
	float radius = 5.0f;
public:
	virtual void OnCollision();

	virtual DirectX::XMFLOAT3 GetWorldPosition() =0;
};


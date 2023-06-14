#include "Bill.h"

void Bill::Init(Model* model, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 billRotation)
{
	model_ = model;

	bill->SetPosition({ position });
	bill->SetModel(model_);
	bill->scale = { scale };
	bill->rotation = { billRotation };
}

void Bill::Update()
{

	bill->Update();
}

void Bill::Draw()
{
	bill->Draw();
}

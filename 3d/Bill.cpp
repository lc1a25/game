#include "Bill.h"

void Bill::Init(Model* model, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale)
{
	model_ = model;

	bill->SetPosition({ position });
	bill->SetModel(model_);
	bill->scale = { scale };
}

void Bill::Update()
{
	//�J�����̌��s�����������
	if (cameraZ >= bill->position.z)
	{
		billDeadFlag = true;
	}
	bill->Update();
}

void Bill::Draw()
{
	bill->Draw();
}
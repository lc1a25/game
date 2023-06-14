#include "BillManager.h"
#include <time.h>

void BillManager::BillSet(Model* model)
{
	model_ = model;
	BillCreate(model_);
}

void BillManager::Update()
{
	bills.remove_if([](std::unique_ptr<Bill>& bill)
		{
			return bill->billDead();
		});

	for (std::unique_ptr<Bill>& bill : bills)
	{
		bill->Update();
	}
}

void BillManager::Draw()
{
	for (std::unique_ptr<Bill>& bill : bills)
	{
		bill->Draw();
	}
}

FLOAT BillManager::BillScaleY(int randam)
{
	if (randam == 1)
	{
		billScaleY = 13;
	}
	else if (randam == 2)
	{
		billScaleY = 7;
	}
	else if (randam == 0)
	{
		billScaleY = 10;
	}
	return billScaleY;
}

DirectX::XMFLOAT3 BillManager::BillRot(int randam)
{
	if (randam == 1)
	{
		billRotation.x = 90;
		billRotation.y = 45;
		billRotation.z = 20;
	}
	else if (randam == 2)
	{
		billRotation.x = -20;
		billRotation.y = -45;
		billRotation.z = -90;
	}
	else if (randam == 0)
	{
		billRotation.x = 90;
		billRotation.y = 0;
		billRotation.z = 45;
	}
	return billRotation;
}

void BillManager::BillCreate(Model* model)
{
	const float iMax = 600;
	const float iAdd = 40;
	const float startZ = 130;
	const float iMaxS = 1000;
	const float iMaxE = 400;
srand((1));
	for (float i = 0; i < iMax; i += iAdd)
	{
		

		randBill = rand() % 3;
	/*	randBillRot = rand() % 15;
		if (randBillRot <= 0)
		{
			billRotation.z = 90;
		}*/
		BillScaleY(randBill);
			//bill¶¬
			std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
			newBill->Init(model_, { 40,-32,startZ + i }, { 4,billScaleY,4 }, billRotation);

			//bill“o˜^
			bills.push_back(std::move(newBill));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß
		
	}
	for (float i = 0; i < iMax; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
			//bill¶¬
			std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
			newBill->Init(model_, { -40,-32,startZ + i }, { 4,billScaleY,4 });

			//bill“o˜^
			bills.push_back(std::move(newBill));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß
		
	}

	for (float i = 0; i < iMax; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
			//bill¶¬
			std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
			newBill->Init(model_, { 80,-32,startZ + i }, { 4,billScaleY,4 });

			//bill“o˜^
			bills.push_back(std::move(newBill));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß
		
	}
	for (float i = 0; i < iMax; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
			//bill¶¬
			std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
			newBill->Init(model_, { -80,-32,startZ + i }, { 4,billScaleY,4 });

			//bill“o˜^
			bills.push_back(std::move(newBill));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß
		
	}
	for (float i = 0; i < iMax; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
			//bill¶¬
			std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
			newBill->Init(model_, { 120,-32,startZ + i }, { 4,billScaleY,4 });

			//bill“o˜^
			bills.push_back(std::move(newBill));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß
		
	}
	for (float i = 0; i < iMax; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
			//bill¶¬
			std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
			newBill->Init(model_, { -120,-32,startZ + i }, { 4,billScaleY,4 });

			//bill“o˜^
			bills.push_back(std::move(newBill));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß
		
	}


	for (float i = 0; i < iMaxS; i += iAdd)
	{


		randBill = rand() % 3;
		/*	randBillRot = rand() % 15;
			if (randBillRot <= 0)
			{
				billRotation.z = 90;
			}*/
		BillScaleY(randBill);
		//bill¶¬
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(model_, { 40,-32,0 - i }, { 4,billScaleY,4 }, billRotation);

		//bill“o˜^
		bills.push_back(std::move(newBill));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß

	}
	for (float i = 0; i < iMaxS; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
		//bill¶¬
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(model_, { -40,-32,0 - i }, { 4,billScaleY,4 });

		//bill“o˜^
		bills.push_back(std::move(newBill));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß

	}

	for (float i = 0; i < iMaxS; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
		//bill¶¬
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(model_, { 80,-32,0 - i }, { 4,billScaleY,4 });

		//bill“o˜^
		bills.push_back(std::move(newBill));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß

	}
	for (float i = 0; i < iMaxS; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
		//bill¶¬
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(model_, { -80,-32,0 - i }, { 4,billScaleY,4 });

		//bill“o˜^
		bills.push_back(std::move(newBill));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß

	}
	for (float i = 0; i < iMaxS; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
		//bill¶¬
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(model_, { 120,-32,0 - i }, { 4,billScaleY,4 });

		//bill“o˜^
		bills.push_back(std::move(newBill));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß

	}
	for (float i = 0; i < iMaxS; i += iAdd)
	{
		randBill = rand() % 3;

		BillScaleY(randBill);
		//bill¶¬
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(model_, { -120,-32,0 - i }, { 4,billScaleY,4 });

		//bill“o˜^
		bills.push_back(std::move(newBill));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß

	}
	

	//ƒ{ƒXí‚Ìƒrƒ‹@ƒ‰ƒ“ƒ_ƒ€‚É“|‚µ‚Ä‰ñ“]‚µ‚Ä”z’u
	for (float i = 0; i < iMaxE; i += iAdd)
	{

		randBill = rand() % 3;
		randBillRot = rand() % 3;

		BillScaleY(randBill);
		billRotation = BillRot(randBillRot);
		//bill¶¬
		std::unique_ptr<Bill> newBill = std::make_unique<Bill>();
		newBill->Init(model_, { -200 + i,-32,990 }, { 4,billScaleY,4 },billRotation);
		
		//bill¶¬
		std::unique_ptr<Bill> newBill2 = std::make_unique<Bill>();
		newBill2->Init(model_, { 200 - i,-32,990 }, { 4,billScaleY,4 }, billRotation);

		//bill¶¬
		std::unique_ptr<Bill> newBill3 = std::make_unique<Bill>();
		newBill3->Init(model_, { 200 + i,-32,1110 }, { 4,billScaleY,4 }, billRotation);

		//bill¶¬
		std::unique_ptr<Bill> newBill4 = std::make_unique<Bill>();
		newBill4->Init(model_, { 200 - i,-32,1110 }, { 4,billScaleY,4 }, billRotation);

		//bill¶¬
		std::unique_ptr<Bill> newBill5 = std::make_unique<Bill>();
		newBill5->Init(model_, { 200 + i,-32,1230 }, { 4,billScaleY,4 }, billRotation);

		//bill¶¬
		std::unique_ptr<Bill> newBill6 = std::make_unique<Bill>();
		newBill6->Init(model_, { 200 - i,-32,1230 }, { 4,billScaleY,4 }, billRotation);


		//bill“o˜^
		bills.push_back(std::move(newBill));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß
		//bill“o˜^
		bills.push_back(std::move(newBill2));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß

		//bill“o˜^
		bills.push_back(std::move(newBill3));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß
		//bill“o˜^
		bills.push_back(std::move(newBill4));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß
		//bill“o˜^
		bills.push_back(std::move(newBill5));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß
		//bill“o˜^
		bills.push_back(std::move(newBill6));//move ‚Íƒ†ƒj[ƒN‚©‚ç÷“n‚·‚é‚½‚ß
	}
}

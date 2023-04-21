#include "Enemy.h"
#pragma once
class BossChild //�{�X�̎�����܂��G
{
	Model* enemyModel_ = nullptr;
	XMFLOAT3 position = { 0,0,0 };

	Enemy* bossChild = nullptr;

	XMFLOAT3 bossVec;//�{�X�̈ړ���

	XMFLOAT3 bossPos;//�{�X�̍��W

	XMFLOAT3 shotRange;

	int childNumber;


	bool bossDrawFlag = true;

	bool bossDead = false;
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="enemyModel">boss�̎���̃��f��</param>
	/// <param name="position">���W</param>
	/// <param name="childNumber">����̔ԍ�</param>
	void Init(Model* enemyModel,XMFLOAT3 position, int childNumber); 

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �e�̔��ˊԊu
	/// </summary>
	/// <param name="shotRange"></param>
	void SetChildShotRange(XMFLOAT3 shotRange) { this->shotRange = shotRange; }

	/// <summary>
	/// �{�X�̍��W���Z�b�g
	/// </summary>
	/// <param name="bossPos">�{�X�̍��W</param>
	void SetBossPos(XMFLOAT3 bossPos) { this->bossPos = bossPos; }

	/// <summary>
	/// �{�X�̈ړ��ʂ��Z�b�g
	/// </summary>
	/// <param name="bossVec">�{�X�̈ړ���</param>
	void SetBossVec(XMFLOAT3 bossVec) { this->bossVec = bossVec; }

	/// <summary>
	/// �{�X�̐������Z�b�g
	/// </summary>
	/// <param name="bossDead">�{�X�̐���</param>
	void SetBossDead(bool bossDead) { this->bossDead = bossDead; }

	/// <summary>
	/// �{�X���o���A�𒣂������Z�b�g
	/// </summary>
	/// <param name="barrierPhaseFlag">�{�X���o���A�𒣂�����</param>
	void SetBarrierPhaseFlag(bool barrierPhaseFlag) { bossChild->SetBarrierPhaseFlag(barrierPhaseFlag); }

	/// <summary>
	/// �{�X�̎�����܂��(����)�����񂾂�
	/// </summary>
	/// <returns>�{�X�̎�����܂��(����)������ł邩</returns>
	bool GetMiniDead() { return bossChild->IsDead(); }

	Enemy* GetEnemy() { return bossChild; }
};


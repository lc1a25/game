#pragma once

#include "Enemy.h"

class Boss
{
	//���f��
	Model* enemyModel_ = nullptr;
	Model* enemyModelMini_ = nullptr;
	Model* barrierModel_ = nullptr;

	//���W
	XMFLOAT3 position = { 0,0,0 };
	Object3d* barrier = barrier->Create();

	//�{�X
	Enemy* boss= nullptr;
	//�{�X�̎���ɂ�����
	Enemy* bossMiniLUF = nullptr;//LUF left up front ����̎�O
	
	
	//�v���C���[�̍��W(�z�[�~���O�p)
	XMFLOAT3 playerWorldPos;

	//�{�X�̈ړ���
	XMFLOAT3 bossVec;
	XMFLOAT3 bossVecTemp;

	//�`��t���O
	bool bossDrawFlag = true;
public:

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="enemyModel">�{�X�̃��f��</param>
	/// <param name="enemyModelMini">�{�X�̎���ɂ����̃��f��</param>
	/// <param name="position">���W</param>
	void Init(Model* enemyModel,Model* enemyModelMini, Model* barrierModel,XMFLOAT3 position);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �v���C���[�̍��W(�z�[�~���O�p)
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void SetPlayerWorldPos(XMFLOAT3 playerPos) { this->playerWorldPos = playerPos; }

	void SetBarrierFlag(bool barrierFlag) { boss->SetBarrierFlag(barrierFlag); }

	/// <summary>
	/// �{�X�̈ړ���
	/// </summary>
	/// <returns>�{�X�̈ړ���</returns>
	XMFLOAT3 GetBossVec() { return bossVec; }


	Enemy* GetEnemy() { return boss; }

	/// <summary>
	/// �{�X�̏ꏊ
	/// </summary>
	/// <returns>�{�X�̏ꏊ</returns>
	XMFLOAT3 GetPos() { return boss->GetWorldPosition(); }

	/// <summary>
	/// �{�X�̐���
	/// </summary>
	/// <returns>�{�X�̐���</returns>
	BOOL GetBossDead() { return boss->IsDead(); }

	BOOL GetBarrierFlag() { return boss->GetBarrierFlag(); }
	BOOL GetBarrierPhaseFlag() { return boss->GetBarrierPhaseFlag(); }
	//�{�X�����[�r�[����
	BOOL GetBossMovieFlag() { return boss->GetBossMovieFlag(); }
};

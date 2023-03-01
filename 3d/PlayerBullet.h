#pragma once
#include "Object3d.h"

class PlayerBullet
{
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private:
	Model* model_ = nullptr;
	Object3d* bullet = bullet->Create();
	XMVECTOR velocity_ = { 0,0,0 };

	//��������
	static const int32_t lifeTime = 60 * 1.5;
	//�f�X�^�C�}�[
	int32_t deathTimer = lifeTime;
	//�f�X�t���O
	bool isDead = false;

	bool enemyIsDead = false;
public:
	
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="playerPosition">���W</param>
	/// <param name="velocity">���ˑ��x</param>
	void Init(Model* model, XMFLOAT3 playerPosition, XMVECTOR velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ������������
	/// </summary>
	void OnCollision();

	/// <summary>
	/// �v���C���[�̐���
	/// </summary>
	/// <returns>�v���C���[�̐���</returns>
	bool IsDead() const { return isDead; }

	/// <summary>
	/// �v���C���[�̍��W
	/// </summary>
	/// <returns>�v���C���[�̍��W</returns>
	XMFLOAT3 GetWorldPosition();
	
	/// <summary>
	/// ���b�N�I�� �����g��Ȃ�
	/// </summary>
	/// <param name="enemyPosition">�G�̍��W</param>
	/// <param name="isDead">�G������ł��邩</param>
	void SetLockOnPosition(XMFLOAT3 enemyPosition, bool isDead);

	/// <summary>
	/// �G�̐���
	/// </summary>
	/// <param name="isDead">�G�̐���</param>
	void SetEnemyDead(bool isDead) { isDead = enemyIsDead; }
};

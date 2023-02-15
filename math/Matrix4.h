
#pragma once
#include <DirectXMath.h>
#include "Vector3.h"

//using namespace DirectX;
class Matrix4
{
	public:	
	//XMMATRIX matrix4;
	
	// �P�ʍs������߂�
	DirectX::XMMATRIX identity();

	// �g��k���s��̐ݒ�
	DirectX::XMMATRIX scale(const Vector3& s);

	// ��]�s��̐ݒ�
	DirectX::XMMATRIX rotateX(float angle);
	DirectX::XMMATRIX rotateY(float angle);
	DirectX::XMMATRIX rotateZ(float angle);

	// ���s�ړ��s��̍쐬
	DirectX::XMMATRIX translate(const Vector3& t);

	// ���W�ϊ��i�x�N�g���ƍs��̊|���Z������j
	static DirectX::XMVECTOR transform(const DirectX::XMVECTOR& v, const DirectX::XMMATRIX& m);

	//�X�N���[�����W���烏�[���h���W��
	static DirectX::XMVECTOR transformScreenToWorld(const DirectX::XMVECTOR& v, const DirectX::XMMATRIX& m);

	// ������Z�q�I�[�o�[���[�h
	static DirectX::XMMATRIX matrixMatrix(DirectX::XMMATRIX& m1, const DirectX::XMMATRIX& m2);

	static DirectX::XMMATRIX matrixInverse(DirectX::XMMATRIX& m);
	
};

//// ������Z�q�I�[�o�[���[�h
//XMMATRIX& operator*=(XMMATRIX& m1, const XMMATRIX& m2);
//
//// 2�����Z�q�I�[�o�[���[�h
//XMMATRIX operator*(const XMMATRIX& m1, const XMMATRIX& m2);
//
//Vector3 operator*(const Vector3& v, const XMMATRIX& m);

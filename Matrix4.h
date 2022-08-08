
#pragma once
#include <DirectXMath.h>
#include "Vector3.h"

using namespace DirectX;
class Matrix4
{
	public:	
	//XMMATRIX matrix4;
	
	// �P�ʍs������߂�
	XMMATRIX identity();

	// �g��k���s��̐ݒ�
	XMMATRIX scale(const Vector3& s);

	// ��]�s��̐ݒ�
	XMMATRIX rotateX(float angle);
	XMMATRIX rotateY(float angle);
	XMMATRIX rotateZ(float angle);

	// ���s�ړ��s��̍쐬
	XMMATRIX translate(const Vector3& t);

	// ���W�ϊ��i�x�N�g���ƍs��̊|���Z������j
	static XMVECTOR transform(const XMVECTOR& v, const XMMATRIX& m);

	// ������Z�q�I�[�o�[���[�h
	static XMMATRIX matrixMatrix(XMMATRIX m1, const XMMATRIX m2);
	
};

// ������Z�q�I�[�o�[���[�h
XMMATRIX& operator*=(XMMATRIX& m1, const XMMATRIX& m2);

// 2�����Z�q�I�[�o�[���[�h
XMMATRIX operator*(const XMMATRIX& m1, const XMMATRIX& m2);

Vector3 operator*(const Vector3& v, const XMMATRIX& m);

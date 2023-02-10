#pragma once
///<summary>
///�����蔻��x�[�X
///</summary>

#include <DirectXMath.h>

using namespace DirectX;
///<summary>
///��
///</summary>
struct Sphere
{
	//���S���W
	XMVECTOR center = { 0,0,0,1 };
	//���a
	float radius = 1.0f;
};

///<summary>
///����
///</summary>
struct Plane
{
	//�@���x�N�g��
	XMVECTOR normal = { 0,1,0,0 };
	//���_(0,0,0)����̋���
	float distance = 0.0f;
};

///<summary>
///�@���t���O�p�`
///</summary>
class Triangle
{
public:
	//���_���W�@3��
	XMVECTOR p0;
	XMVECTOR p1;
	XMVECTOR p2;

	//�@���x�N�g��
	XMVECTOR normal;

	///<summary>
	///�@���̌v�Z	
	///</summary>
	void ComputeNormal();
};

///<summary>
///���C
///</summary>
class Ray
{
public:
	//�n�_���W
	XMVECTOR start = { 0,0,0,1 };
	//����
	XMVECTOR dir = { 1,0,0,0 };
};





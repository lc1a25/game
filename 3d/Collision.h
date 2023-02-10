#pragma once

#include "CollisionBase.h"

///<summary>
///�����蔻��w���p�[�N���X
///</summary>
class Collision
{
public:
	///<summary>
	///���ƕ��ʂ̓����蔻��
	///</summary>
	///<param name = "sphere">��</param>
	/// <param name = "plane">����</param>
	/// <param name = "inter">��_</param>
	/// <returns>�������Ă��邩�ۂ�</param>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, XMVECTOR* inter = nullptr);

	///<summary>
	///�_�ƎO�p�`�̍ŋߐړ_�����߂�
	///</summary>
	///<param name = "point">�_</param>
	/// <param name = "triangle">�O�p�`</param>
	/// <param name = "closest">�ŋߐړ_ (�o�͗p)</param>
	static void ClodestPtPoint2Triangle(const XMVECTOR& point, const Triangle& triangle, XMVECTOR* closest);

	///<summary>
	///���Ɩ@���t���O�p�`�̓����蔻��
	///</summary>
	///<param name = "sphere">��</param>
	/// <param name = "triangle">�O�p�`</param>
	/// <param name = "inter">��_ (�O�p�`��̍ŋߐړ_)</param>
	/// <returns>�������Ă��邩�ۂ�</param>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, XMVECTOR* inter = nullptr);

	///<summary>
	///���C�ƕ��ʂ̓����蔻��
	///</summary>
	///<param name = "lay">���C</param>
	/// <param name = "plane">����</param>
	/// <param name = "inter">���� (�o�͗p)</param>
	/// <param name = "inter">��_ (�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</param>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, XMVECTOR* inter = nullptr);

	///<summary>
	///���C�Ɩ@���t���O�p�`�̓����蔻��
	///</summary>
	///<param name = "lay">���C</param>
	/// <param name = "Triangle">�O�p�`</param>
	/// <param name = "inter">���� (�o�͗p)</param>
	/// <param name = "inter">��_ (�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</param>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, XMVECTOR* inter = nullptr);

	///<summary>
	///���C�Ƌ��̓����蔻��
	///</summary>
	///<param name = "lay">���C</param>
	/// <param name = "sphere">��</param>
	/// <param name = "inter">���� (�o�͗p)</param>
	/// <param name = "inter">��_ (�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</param>
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, XMVECTOR* inter = nullptr);
};
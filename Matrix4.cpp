#include "Matrix4.h"
#include "Vector3.h"
#include <cmath>

XMMATRIX Matrix4::identity()
{
	static const XMMATRIX result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return result;
}

XMMATRIX  Matrix4::scale(const Vector3 &s)
{
	XMMATRIX result
	{
		s.x,0.0f,0.0f,0.0f,
		0.0f,s.y,0.0f,0.0f,
		0.0f,0.0f,s.z,0.0f,
		0.0f,0.0f,0.0f,1.0f

	};
	return result;
}

XMMATRIX  Matrix4::rotateX(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	XMMATRIX result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cos ,sin ,0.0f,
		0.0f,-sin,cos ,0.0f,
		0.0f,0.0f,0.0f,1.0f

	};

	return result;
}

XMMATRIX  Matrix4::rotateY(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	XMMATRIX result
	{
		cos ,0.0f,-sin,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		sin ,0.0f,cos ,0.0f,
		0.0f,0.0f,0.0f,1.0f

	};

	return result;
}

XMMATRIX  Matrix4::rotateZ(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	XMMATRIX result
	{
		cos,sin,0.0f,0.0f,
		-sin,cos,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f

	};

	return result;
}

XMMATRIX  Matrix4::translate(const Vector3 &t)
{
	XMMATRIX result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		t.x,t.y,t.z,1.0f

	};

	return result;
}

XMVECTOR  Matrix4::transform(const XMVECTOR&v, const XMMATRIX&m)
{
	float w = v.m128_f32[0] * m.r[0].m128_f32[3] + v.m128_f32[1] * m.r[1].m128_f32[3] + v.m128_f32[2] * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];

	XMVECTOR result
	{
		(v.m128_f32[0] * m.r[0].m128_f32[0] + v.m128_f32[1] * m.r[1].m128_f32[0] + v.m128_f32[2] * m.r[2].m128_f32[0]) / w,
		(v.m128_f32[0] * m.r[0].m128_f32[1] + v.m128_f32[1] * m.r[1].m128_f32[1] + v.m128_f32[2] * m.r[2].m128_f32[1]) / w,
		(v.m128_f32[0] * m.r[0].m128_f32[2] + v.m128_f32[1] * m.r[1].m128_f32[2] + v.m128_f32[2] * m.r[2].m128_f32[2]) / w

	};
	return result;
}

XMMATRIX Matrix4::matrixMatrix(XMMATRIX& m1, const XMMATRIX& m2)
{
	XMMATRIX result =
	{ 0.0f,0.0f,0.0f,0.0f,
	  0.0f,0.0f,0.0f,0.0f,
	  0.0f,0.0f,0.0f,0.0f,
	  0.0f,0.0f,0.0f,0.0f };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.r[i].m128_f32[j] += m1.r[i].m128_f32[k] * m2.r[k].m128_f32[j];
			}
		}
	}
	m1 = result;

	return m1;
}

XMMATRIX Matrix4::matrixInverse(XMMATRIX& m)
{
	/*XMMATRIX Identity = { 1,0,0,0,
						    0,1,0,0,
						    0,0,1,0,
						    0,0,0,1 };*/
	float mInverse = 
		+m.r[0].m128_f32[0] * (
			(m.r[1].m128_f32[1] * m.r[2].m128_f32[2] * m.r[3].m128_f32[3]) +
			(m.r[1].m128_f32[2] * m.r[2].m128_f32[3] * m.r[3].m128_f32[1]) +
			(m.r[1].m128_f32[3] * m.r[2].m128_f32[1] * m.r[3].m128_f32[2]) -
			(m.r[1].m128_f32[1] * m.r[2].m128_f32[3] * m.r[3].m128_f32[2]) -
			(m.r[1].m128_f32[2] * m.r[2].m128_f32[1] * m.r[3].m128_f32[3]) -
			(m.r[1].m128_f32[3] * m.r[2].m128_f32[2] * m.r[3].m128_f32[1]))

		- m.r[0].m128_f32[1] * (
			(m.r[1].m128_f32[0] * m.r[2].m128_f32[2] * m.r[3].m128_f32[3]) +
			(m.r[1].m128_f32[2] * m.r[2].m128_f32[3] * m.r[3].m128_f32[0]) +
			(m.r[1].m128_f32[3] * m.r[2].m128_f32[0] * m.r[3].m128_f32[2]) -
			(m.r[1].m128_f32[0] * m.r[2].m128_f32[3] * m.r[3].m128_f32[2]) -
			(m.r[1].m128_f32[2] * m.r[2].m128_f32[0] * m.r[3].m128_f32[3]) -
			(m.r[1].m128_f32[3] * m.r[2].m128_f32[2] * m.r[3].m128_f32[0]))

		+ m.r[0].m128_f32[2] * (
			(m.r[1].m128_f32[0] * m.r[2].m128_f32[1] * m.r[3].m128_f32[3]) +
			(m.r[1].m128_f32[1] * m.r[2].m128_f32[3] * m.r[3].m128_f32[0]) +
			(m.r[1].m128_f32[3] * m.r[2].m128_f32[0] * m.r[3].m128_f32[1]) -
			(m.r[1].m128_f32[0] * m.r[2].m128_f32[3] * m.r[3].m128_f32[1]) -
			(m.r[1].m128_f32[1] * m.r[2].m128_f32[0] * m.r[3].m128_f32[3]) -
			(m.r[1].m128_f32[3] * m.r[2].m128_f32[1] * m.r[3].m128_f32[0]))

		- m.r[0].m128_f32[3] * (
			(m.r[1].m128_f32[0] * m.r[2].m128_f32[1] * m.r[3].m128_f32[2]) +
			(m.r[1].m128_f32[1] * m.r[2].m128_f32[2] * m.r[3].m128_f32[0]) +
			(m.r[1].m128_f32[2] * m.r[2].m128_f32[0] * m.r[3].m128_f32[1]) -
			(m.r[1].m128_f32[0] * m.r[2].m128_f32[2] * m.r[3].m128_f32[1]) -
			(m.r[1].m128_f32[1] * m.r[2].m128_f32[0] * m.r[3].m128_f32[2]) -
			(m.r[1].m128_f32[2] * m.r[2].m128_f32[1] * m.r[3].m128_f32[0]));

	//a11		   
	m.r[0].m128_f32[0] = 
		(m.r[1].m128_f32[1] * m.r[2].m128_f32[2] * m.r[3].m128_f32[3]) +
		(m.r[1].m128_f32[2] * m.r[2].m128_f32[3] * m.r[3].m128_f32[1]) +
		(m.r[1].m128_f32[3] * m.r[2].m128_f32[1] * m.r[3].m128_f32[2]) -
		(m.r[1].m128_f32[1] * m.r[2].m128_f32[3] * m.r[3].m128_f32[2]) -
		(m.r[1].m128_f32[2] * m.r[2].m128_f32[1] * m.r[3].m128_f32[3]) -
		(m.r[1].m128_f32[3] * m.r[2].m128_f32[2] * m.r[3].m128_f32[1]);

	//a12
	m.r[0].m128_f32[1] = -1 * (
		(m.r[1].m128_f32[0] * m.r[2].m128_f32[2] * m.r[3].m128_f32[3]) +
		(m.r[1].m128_f32[2] * m.r[2].m128_f32[3] * m.r[3].m128_f32[0]) +
		(m.r[1].m128_f32[3] * m.r[2].m128_f32[0] * m.r[3].m128_f32[2]) -
		(m.r[1].m128_f32[0] * m.r[2].m128_f32[3] * m.r[3].m128_f32[2]) -
		(m.r[1].m128_f32[2] * m.r[2].m128_f32[0] * m.r[3].m128_f32[3]) -
		(m.r[1].m128_f32[3] * m.r[2].m128_f32[2] * m.r[3].m128_f32[0]));
	//a13
	m.r[0].m128_f32[2] =
		(m.r[1].m128_f32[0] * m.r[2].m128_f32[1] * m.r[3].m128_f32[3]) +
		(m.r[1].m128_f32[1] * m.r[2].m128_f32[3] * m.r[3].m128_f32[0]) +
		(m.r[1].m128_f32[3] * m.r[2].m128_f32[0] * m.r[3].m128_f32[1]) -
		(m.r[1].m128_f32[0] * m.r[2].m128_f32[3] * m.r[3].m128_f32[1]) -
		(m.r[1].m128_f32[1] * m.r[2].m128_f32[0] * m.r[3].m128_f32[3]) -
		(m.r[1].m128_f32[3] * m.r[2].m128_f32[1] * m.r[3].m128_f32[0]);
	//a14
	m.r[0].m128_f32[3] = -1 * (
		(m.r[1].m128_f32[0] * m.r[2].m128_f32[1] * m.r[3].m128_f32[2]) +
		(m.r[1].m128_f32[1] * m.r[2].m128_f32[2] * m.r[3].m128_f32[0]) +
		(m.r[1].m128_f32[2] * m.r[2].m128_f32[0] * m.r[3].m128_f32[1]) -
		(m.r[1].m128_f32[0] * m.r[2].m128_f32[2] * m.r[3].m128_f32[1]) -
		(m.r[1].m128_f32[1] * m.r[2].m128_f32[0] * m.r[3].m128_f32[2]) -
		(m.r[1].m128_f32[2] * m.r[2].m128_f32[1] * m.r[3].m128_f32[0]));
	//a21
	m.r[1].m128_f32[0] = -1 * (
		(m.r[0].m128_f32[1] * m.r[2].m128_f32[2] * m.r[3].m128_f32[3]) +
		(m.r[0].m128_f32[2] * m.r[2].m128_f32[3] * m.r[3].m128_f32[1]) +
		(m.r[0].m128_f32[3] * m.r[2].m128_f32[1] * m.r[3].m128_f32[2]) -
		(m.r[0].m128_f32[1] * m.r[2].m128_f32[3] * m.r[3].m128_f32[2]) -
		(m.r[0].m128_f32[2] * m.r[2].m128_f32[1] * m.r[3].m128_f32[3]) -
		(m.r[0].m128_f32[3] * m.r[2].m128_f32[2] * m.r[3].m128_f32[1]));
	//a22
	m.r[1].m128_f32[1] = 
		(m.r[0].m128_f32[0] * m.r[2].m128_f32[2] * m.r[3].m128_f32[3]) +
		(m.r[0].m128_f32[2] * m.r[2].m128_f32[3] * m.r[3].m128_f32[0]) +
		(m.r[0].m128_f32[3] * m.r[2].m128_f32[0] * m.r[3].m128_f32[2]) -
		(m.r[0].m128_f32[0] * m.r[2].m128_f32[3] * m.r[3].m128_f32[2]) -
		(m.r[0].m128_f32[2] * m.r[2].m128_f32[0] * m.r[3].m128_f32[3]) -
		(m.r[0].m128_f32[3] * m.r[2].m128_f32[2] * m.r[3].m128_f32[0]);
	//a23
	m.r[1].m128_f32[2] = -1 * (
		(m.r[0].m128_f32[0] * m.r[2].m128_f32[1] * m.r[3].m128_f32[3]) +
		(m.r[0].m128_f32[1] * m.r[2].m128_f32[3] * m.r[3].m128_f32[0]) +
		(m.r[0].m128_f32[3] * m.r[2].m128_f32[0] * m.r[3].m128_f32[1]) -
		(m.r[0].m128_f32[0] * m.r[2].m128_f32[3] * m.r[3].m128_f32[1]) -
		(m.r[0].m128_f32[1] * m.r[2].m128_f32[0] * m.r[3].m128_f32[3]) -
		(m.r[0].m128_f32[3] * m.r[2].m128_f32[1] * m.r[3].m128_f32[0]));
	//a24
	m.r[1].m128_f32[3] =
		(m.r[0].m128_f32[0] * m.r[2].m128_f32[1] * m.r[3].m128_f32[2]) +
		(m.r[0].m128_f32[1] * m.r[2].m128_f32[2] * m.r[3].m128_f32[0]) +
		(m.r[0].m128_f32[2] * m.r[2].m128_f32[0] * m.r[3].m128_f32[1]) -
		(m.r[0].m128_f32[0] * m.r[2].m128_f32[2] * m.r[3].m128_f32[1]) -
		(m.r[0].m128_f32[1] * m.r[2].m128_f32[0] * m.r[3].m128_f32[2]) -
		(m.r[0].m128_f32[2] * m.r[2].m128_f32[1] * m.r[3].m128_f32[0]);
	//a31
	m.r[2].m128_f32[0] =
		(m.r[0].m128_f32[0] * m.r[1].m128_f32[2] * m.r[3].m128_f32[3]) +
		(m.r[0].m128_f32[2] * m.r[1].m128_f32[3] * m.r[3].m128_f32[1]) +
		(m.r[0].m128_f32[3] * m.r[1].m128_f32[1] * m.r[3].m128_f32[2]) -
		(m.r[0].m128_f32[1] * m.r[1].m128_f32[3] * m.r[3].m128_f32[2]) -
		(m.r[0].m128_f32[2] * m.r[1].m128_f32[1] * m.r[3].m128_f32[3]) -
		(m.r[0].m128_f32[3] * m.r[1].m128_f32[2] * m.r[3].m128_f32[1]);
	//a32
	m.r[2].m128_f32[1] = -1 * (
		(m.r[0].m128_f32[0] * m.r[1].m128_f32[2] * m.r[3].m128_f32[3]) +
		(m.r[0].m128_f32[2] * m.r[1].m128_f32[3] * m.r[3].m128_f32[0]) +
		(m.r[0].m128_f32[3] * m.r[1].m128_f32[0] * m.r[3].m128_f32[2]) -
		(m.r[0].m128_f32[0] * m.r[1].m128_f32[3] * m.r[3].m128_f32[2]) -
		(m.r[0].m128_f32[2] * m.r[1].m128_f32[0] * m.r[3].m128_f32[3]) -
		(m.r[0].m128_f32[3] * m.r[1].m128_f32[2] * m.r[3].m128_f32[0]));
	//a33
	m.r[2].m128_f32[2] =
		(m.r[0].m128_f32[0] * m.r[1].m128_f32[1] * m.r[3].m128_f32[3]) +
		(m.r[0].m128_f32[1] * m.r[1].m128_f32[3] * m.r[3].m128_f32[0]) +
		(m.r[0].m128_f32[3] * m.r[1].m128_f32[0] * m.r[3].m128_f32[1]) -
		(m.r[0].m128_f32[0] * m.r[1].m128_f32[3] * m.r[3].m128_f32[1]) -
		(m.r[0].m128_f32[1] * m.r[1].m128_f32[0] * m.r[3].m128_f32[3]) -
		(m.r[0].m128_f32[3] * m.r[1].m128_f32[1] * m.r[3].m128_f32[0]);
	//a34
	m.r[2].m128_f32[3] = -1 * (
		(m.r[0].m128_f32[0] * m.r[1].m128_f32[1] * m.r[3].m128_f32[2]) +
		(m.r[0].m128_f32[1] * m.r[1].m128_f32[2] * m.r[3].m128_f32[0]) +
		(m.r[0].m128_f32[2] * m.r[1].m128_f32[0] * m.r[3].m128_f32[1]) -
		(m.r[0].m128_f32[0] * m.r[1].m128_f32[2] * m.r[3].m128_f32[1]) -
		(m.r[0].m128_f32[1] * m.r[1].m128_f32[0] * m.r[3].m128_f32[2]) -
		(m.r[0].m128_f32[2] * m.r[1].m128_f32[1] * m.r[3].m128_f32[0]));
	//a41
	m.r[3].m128_f32[0] = -1 * (
		(m.r[0].m128_f32[1] * m.r[1].m128_f32[2] * m.r[2].m128_f32[3]) +
		(m.r[0].m128_f32[2] * m.r[1].m128_f32[3] * m.r[2].m128_f32[1]) +
		(m.r[0].m128_f32[3] * m.r[1].m128_f32[1] * m.r[2].m128_f32[2]) -
		(m.r[0].m128_f32[1] * m.r[1].m128_f32[3] * m.r[2].m128_f32[2]) -
		(m.r[0].m128_f32[2] * m.r[1].m128_f32[1] * m.r[2].m128_f32[3]) -
		(m.r[0].m128_f32[3] * m.r[1].m128_f32[2] * m.r[2].m128_f32[1]));
	//a42
	m.r[3].m128_f32[1] =
		(m.r[0].m128_f32[0] * m.r[1].m128_f32[2] * m.r[2].m128_f32[3]) +
		(m.r[0].m128_f32[2] * m.r[1].m128_f32[3] * m.r[2].m128_f32[0]) +
		(m.r[0].m128_f32[3] * m.r[1].m128_f32[0] * m.r[2].m128_f32[2]) -
		(m.r[0].m128_f32[0] * m.r[1].m128_f32[3] * m.r[2].m128_f32[2]) -
		(m.r[0].m128_f32[2] * m.r[1].m128_f32[0] * m.r[2].m128_f32[3]) -
		(m.r[0].m128_f32[3] * m.r[1].m128_f32[2] * m.r[2].m128_f32[0]);
	//a43
	m.r[3].m128_f32[2] = -1 * (
		(m.r[0].m128_f32[0] * m.r[1].m128_f32[1] * m.r[2].m128_f32[3]) +
		(m.r[0].m128_f32[1] * m.r[1].m128_f32[3] * m.r[2].m128_f32[0]) +
		(m.r[0].m128_f32[3] * m.r[1].m128_f32[0] * m.r[2].m128_f32[1]) -
		(m.r[0].m128_f32[0] * m.r[1].m128_f32[3] * m.r[2].m128_f32[1]) -
		(m.r[0].m128_f32[1] * m.r[1].m128_f32[0] * m.r[2].m128_f32[3]) -
		(m.r[0].m128_f32[3] * m.r[1].m128_f32[1] * m.r[2].m128_f32[0]));
	//a44
	m.r[3].m128_f32[3] =
		(m.r[0].m128_f32[0] * m.r[1].m128_f32[1] * m.r[2].m128_f32[2]) +
		(m.r[0].m128_f32[1] * m.r[1].m128_f32[2] * m.r[2].m128_f32[0]) +
		(m.r[0].m128_f32[2] * m.r[1].m128_f32[0] * m.r[2].m128_f32[1]) -
		(m.r[0].m128_f32[0] * m.r[1].m128_f32[2] * m.r[2].m128_f32[1]) -
		(m.r[0].m128_f32[1] * m.r[1].m128_f32[0] * m.r[2].m128_f32[2]) -
		(m.r[0].m128_f32[2] * m.r[1].m128_f32[1] * m.r[2].m128_f32[0]);
			   
		 return mInverse * (m);
}



XMMATRIX& operator*=(XMMATRIX& m1, const XMMATRIX& m2)
{
	
	XMMATRIX result =
	  { 0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,0.0f };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.r[i].m128_f32[j] += m1.r[i].m128_f32[k] * m2.r[k].m128_f32[j];
			}
		}
	}
	m1 = result;

	return m1;
}

XMMATRIX operator*(const XMMATRIX& m1, const XMMATRIX& m2)
{
	return XMMATRIX();
}

Vector3 operator*(const Vector3 &v, const XMMATRIX&m)
{
	return transform(v, m);
}



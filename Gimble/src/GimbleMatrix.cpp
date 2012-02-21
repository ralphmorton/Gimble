
#include "GimbleMatrix.h"

namespace Gimble
{
	Matrix::Matrix(const Quaternion &q)
	{
	    float num9 = q.x * q.x;
	    float num8 = q.y * q.y;
	    float num7 = q.z * q.z;
	    float num6 = q.x * q.y;
	    float num5 = q.z * q.w;
	    float num4 = q.z * q.x;
	    float num3 = q.y * q.w;
	    float num2 = q.y * q.z;
	    float num = q.x * q.w;
	    m[0][0] = 1.f - (2.f * (num8 + num7));
	    m[0][1] = 2.f * (num6 + num5);
	    m[0][2] = 2.f * (num4 - num3);
	    m[0][3] = 0.f;
	    m[1][0] = 2.f * (num6 - num5);
	    m[1][1] = 1.f - (2.f * (num7 + num9));
	    m[1][2] = 2.f * (num2 + num);
	    m[1][3] = 0.f;
	    m[2][0] = 2.f * (num4 + num3);
	    m[2][1] = 2.f * (num2 - num);
	    m[2][2] = 1.f - (2.f * (num8 + num9));
	    m[2][3] = 0.f;
	    m[3][0] = 0.f;
	    m[3][1] = 0.f;
	    m[3][2] = 0.f;
	    m[3][3] = 1.f;
	}

	Matrix Matrix::inverse() const 
	{
		Matrix matrix2;
		float num5 = m[0][0];
		float num4 = m[0][1];
		float num3 = m[0][2];
		float num2 = m[0][3];
		float num9 = m[1][0];
		float num8 = m[1][1];
		float num7 = m[1][2];
		float num6 = m[1][3];
		float num17 = m[2][0];
		float num16 = m[2][1];
		float num15 = m[2][2];
		float num14 = m[2][3];
		float num13 = m[3][0];
		float num12 = m[3][1];
		float num11 = m[3][2];
		float num10 = m[3][3];

		float num23 = (num15 * num10) - (num14 * num11);
		float num22 = (num16 * num10) - (num14 * num12);
		float num21 = (num16 * num11) - (num15 * num12);
		float num20 = (num17 * num10) - (num14 * num13);
		float num19 = (num17 * num11) - (num15 * num13);
		float num18 = (num17 * num12) - (num16 * num13);
		float num39 = ((num8 * num23) - (num7 * num22)) + (num6 * num21);
		float num38 = -(((num9 * num23) - (num7 * num20)) + (num6 * num19));
		float num37 = ((num9 * num22) - (num8 * num20)) + (num6 * num18);
		float num36 = -(((num9 * num21) - (num8 * num19)) + (num7 * num18));
		float num = 1.f / ((((num5 * num39) + (num4 * num38)) + (num3 * num37)) + (num2 * num36));

		matrix2.m[0][0] = num39 * num;
		matrix2.m[1][0] = num38 * num;
		matrix2.m[2][0] = num37 * num;
		matrix2.m[3][0] = num36 * num;
		matrix2.m[0][1] = -(((num4 * num23) - (num3 * num22)) + (num2 * num21)) * num;
		matrix2.m[1][1] = (((num5 * num23) - (num3 * num20)) + (num2 * num19)) * num;
		matrix2.m[2][1] = -(((num5 * num22) - (num4 * num20)) + (num2 * num18)) * num;
		matrix2.m[3][1] = (((num5 * num21) - (num4 * num19)) + (num3 * num18)) * num;

		float num35 = (num7 * num10) - (num6 * num11);
		float num34 = (num8 * num10) - (num6 * num12);
		float num33 = (num8 * num11) - (num7 * num12);
		float num32 = (num9 * num10) - (num6 * num13);
		float num31 = (num9 * num11) - (num7 * num13);
		float num30 = (num9 * num12) - (num8 * num13);

		matrix2.m[0][2] = (((num4 * num35) - (num3 * num34)) + (num2 * num33)) * num;
		matrix2.m[1][2] = -(((num5 * num35) - (num3 * num32)) + (num2 * num31)) * num;
		matrix2.m[2][2] = (((num5 * num34) - (num4 * num32)) + (num2 * num30)) * num;
		matrix2.m[3][2] = -(((num5 * num33) - (num4 * num31)) + (num3 * num30)) * num;

		float num29 = (num7 * num14) - (num6 * num15);
		float num28 = (num8 * num14) - (num6 * num16);
		float num27 = (num8 * num15) - (num7 * num16);
		float num26 = (num9 * num14) - (num6 * num17);
		float num25 = (num9 * num15) - (num7 * num17);
		float num24 = (num9 * num16) - (num8 * num17);

		matrix2.m[0][3] = -(((num4 * num29) - (num3 * num28)) + (num2 * num27)) * num;
		matrix2.m[1][3] = (((num5 * num29) - (num3 * num26)) + (num2 * num25)) * num;
		matrix2.m[2][3] = -(((num5 * num28) - (num4 * num26)) + (num2 * num24)) * num;
		matrix2.m[3][3] = (((num5 * num27) - (num4 * num25)) + (num3 * num24)) * num;

		return matrix2;
	}

	Matrix Matrix::inverse33() const
	{
		Matrix ret = Matrix::IDENTITY;
        float t4 = m[0][0] * m[1][1];
        float t6 = m[0][0] * m[2][1];
        float t8 = m[1][0] * m[0][1];
        float t10 = m[2][0] * m[0][1];
        float t12 = m[1][0] * m[0][2];
        float t14 = m[2][0] * m[0][2];

        float t16 = ( t4 * m[2][2] - t6 * m[1][2] - t8 * m[2][2] + t10 * m[1][2] + t12 * m[2][1] - t14 * m[1][1]);

		if (t16 == 0.0f) return Matrix::IDENTITY;
        float t17 = 1 / t16;

        ret[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * t17;
        ret[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * t17;
        ret[2][0] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * t17;
        ret[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * t17;
        ret[1][1] = (m[0][0] * m[2][2] - t14) * t17;
        ret[2][1] = -(t6 - t10) * t17;
        ret[0][2] = (m[0][1] * m[1][2] - m[1][1] * m[0][2]) * t17;
        ret[1][2] = -(m[0][0] * m[1][2] - t12) * t17;
        ret[2][2] = (t4 - t8) * t17;

        return ret;
	}

	Matrix Matrix::createRotationX(const float &angle)
	{
	    float cos = Math::Cos(angle);
	    float sin = Math::Sin(angle);

		return Matrix(
			1,0,0,0,
			0,cos,sin,0,
			0,-sin,cos,0,
			0,0,0,1);
	}

	Matrix Matrix::createRotationY(const float &angle)
	{
		float cos = Math::Cos(angle);
	    float sin = Math::Sin(angle);

		return Matrix(
			cos,0,-sin,0,
			0,1,0,0,
			sin,0,cos,0,
			0,0,0,1);
	}

	Matrix Matrix::createRotationZ(const float &angle)
	{
		float cos = Math::Cos(angle);
	    float sin = Math::Sin(angle);

		return Matrix(
			cos,sin,0,0,
			-sin,cos,0,0,
			0,0,1,0,
			0,0,0,1);
	}

	Matrix Matrix::createScale(const Vector3 &scale)
	{
		return Matrix(
			scale.x,0,0,0,
			0,scale.y,0,0,
			0,0,scale.z,0,
			0,0,0,1);
	}

	Matrix Matrix::createScale(const float &scaleX, const float &scaleY, const float &scaleZ)
	{
		return Matrix(
			scaleX,0,0,0,
			0,scaleY,0,0,
			0,0,scaleZ,0,
			0,0,0,1);
	}

	Matrix Matrix::createTranslation(const Vector3 &trans)
	{
		return Matrix(
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			trans.x,trans.y,trans.z,1);
	}

	Matrix Matrix::createTranslation(const float &transX, const float &transY, const float &transZ)
	{
		return Matrix(
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			transX,transY,transZ,1);
	}

	Matrix Matrix::createLookAt(const Vector3 &position, const Vector3 &target, const Vector3 &up)
	{
		Vector3 dir = position - target;
		dir.normalise();

		Vector3 v2 = up.cross(dir);
		v2.normalise();

		Vector3 v3 = dir.cross(v2);

		return Matrix(
			v2.x, v3.x, dir.x, 0,
			v2.y, v3.y, dir.y, 0,
			v2.z, v3.z, dir.z, 0,
			-(v2.dot(position)), -(v3.dot(position)), -(dir.dot(position)), 1);
	}

	Matrix Matrix::createPerspectiveFOV(const float &fov, const float &aspectRatio, const float &nearPlane, const float &farPlane)
	{
		assert(fov > 0.0f && fov <= Math::PI);
		assert(nearPlane > 0.0f);
		assert(farPlane > nearPlane);

		float invtanf = 1.0f / Math::Tan(fov * 0.5f);

		return Matrix(
			invtanf/aspectRatio,0,0,0,
			0,invtanf,0,0,
			0,0,farPlane/(nearPlane-farPlane),-1,
			0,0,(nearPlane*farPlane)/(nearPlane-farPlane),0);
	}

	const Matrix Matrix::ZERO(
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0);

    const Matrix Matrix::IDENTITY(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
}
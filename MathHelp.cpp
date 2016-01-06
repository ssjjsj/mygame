#include "Common\d3dApp.h"
#include "MathHelp.h"

namespace MathUntil
{
	XMFLOAT3 lerpFloat3(XMFLOAT3 left, XMFLOAT3 right, float v1, float v2, float v3)
	{
		XMFLOAT3 value;
		value.x = lerp(left.x, right.x, v1, v2, v3);
		value.y = lerp(left.y, right.y, v1, v2, v3);
		value.z = lerp(left.z, right.z, v1, v2, v3);

		return value;
	}

	float lerp(float left, float right, float v1, float v2, float v3)
	{
		if (v3 - v1 < 0.0001f)
			return left;
		float value = left + (right - left)*(v2 - v1) / (v3 - v1);
		return value;
	}

	XMFLOAT4X4 GetTransformMatrix(XMFLOAT3 translate, XMFLOAT3 axis, float angle)
	{
		XMMATRIX m1 = XMMatrixTranslation(translate.x, translate.y, translate.z);
		XMMATRIX m2 = XMMatrixRotationAxis(XMLoadFloat3(&axis), angle);

		XMFLOAT4X4 result;
		XMStoreFloat4x4(&result, m2*m1);
		
		return result;
	}


	XMFLOAT3 quaternionVector (const XMFLOAT4 &q, const XMFLOAT3& v)
	{
		// nVidia SDK implementation
		XMFLOAT3 uv, uuv;
		XMFLOAT3 qvec(q.x, q.y, q.z);
		uv = crossProduct(qvec, v);
		uuv = crossProduct(qvec, uv);
		scale(uv, 2.0f*q.w);
		scale(uuv, 2.0f);

		return XMFLOAT3(v.x + uv.x + uuv.x,
			v.y + uv.y + uuv.y, v.z + uv.z + uuv.z);
	}


	XMFLOAT3 crossProduct(const XMFLOAT3& lkVector, const XMFLOAT3& rkVector)
	{
		return XMFLOAT3(
			lkVector.y * rkVector.z - lkVector.z * rkVector.y,
			lkVector.z * rkVector.x - lkVector.x * rkVector.z,
			lkVector.x * rkVector.y - lkVector.y * rkVector.x);
	}

	XMFLOAT3 scale(XMFLOAT3 v, float s)
	{
		v.x *= s;
		v.y *= s;
		v.z *= s;

		return XMFLOAT3(v);
	}


	XMFLOAT3X3 QuaternionToRotationMatrix(XMFLOAT4 q)
	{
		XMFLOAT3X3 kRot;
		float fTx = q.x + q.x;
		float fTy = q.y + q.y;
		float fTz = q.z + q.z;
		float fTwx = fTx*q.w;
		float fTwy = fTy*q.w;
		float fTwz = fTz*q.w;
		float fTxx = fTx*q.x;
		float fTxy = fTy*q.x;
		float fTxz = fTz*q.x;
		float fTyy = fTy*q.y;
		float fTyz = fTz*q.y;
		float fTzz = fTz*q.z;

		kRot._11 = 1.0f - (fTyy + fTzz);
		kRot._12 = fTxy - fTwz;
		kRot._13 = fTxz + fTwy;
		kRot._21 = fTxy + fTwz;
		kRot._22 = 1.0f - (fTxx + fTzz);
		kRot._23 = fTyz - fTwx;
		kRot._31 = fTxz - fTwy;
		kRot._32 = fTyz + fTwx;
		kRot._33 = 1.0f - (fTxx + fTyy);

		return kRot;
	}

	XMFLOAT4 vectorMupilyMatrix(XMFLOAT4 v, XMFLOAT4X4 m)
	{
		XMFLOAT4 result;

		result.x = m._11*v.x + m._12*v.y + m._13*v.z + m._14*v.w;
		result.y = m._21*v.x + m._22*v.y + m._23*v.z + m._24*v.w;
		result.z = m._31*v.x + m._32*v.y + m._33*v.z + m._34*v.w;
		result.w = m._41*v.x + m._42*v.y + m._43*v.z + m._44*v.w;

		return result;
	}

	XMFLOAT4 QuaternionMupilyQuaternion(const XMFLOAT4& lkQ, const XMFLOAT4& rkQ)
	{
		// NOTE:  Multiplication is not generally commutative, so in most
		// cases p*q != q*p.

		return XMFLOAT4
			(
			lkQ.w * rkQ.w - lkQ.x * rkQ.x - lkQ.y * rkQ.y - lkQ.z * rkQ.z,
			lkQ.w * rkQ.x + lkQ.x * rkQ.w + lkQ.y * rkQ.z - lkQ.z * rkQ.y,
			lkQ.w * rkQ.y + lkQ.y * rkQ.w + lkQ.z * rkQ.x - lkQ.x * rkQ.z,
			lkQ.w * rkQ.z + lkQ.z * rkQ.w + lkQ.x * rkQ.y - lkQ.y * rkQ.x
			);
	}
}
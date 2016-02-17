#include <windows.h>
#include <xnamath.h>
#include <cstdio>

namespace MathUntil
{
	XMFLOAT3 lerpFloat3(XMFLOAT3 left, XMFLOAT3 right, float v1, float v2, float v3);

	float lerp(float left, float right, float v1, float v2, float v3);

	XMFLOAT4X4 GetTransformMatrix(XMFLOAT3 tranlate, XMFLOAT3 axis, float angle);

	XMFLOAT3 quaternionVector(const XMFLOAT4 &q, const XMFLOAT3& v);

	XMFLOAT3 crossProduct(const XMFLOAT3& lkVector, const XMFLOAT3& rkVector);

	XMFLOAT3 scale(XMFLOAT3 v, float s);

	XMFLOAT3X3 QuaternionToRotationMatrix(XMFLOAT4 q);

	XMFLOAT4 vectorMupilyMatrix(XMFLOAT4 v, XMFLOAT4X4 m);

	XMFLOAT4 QuaternionMupilyQuaternion(const XMFLOAT4& lkQ, const XMFLOAT4& rkQ);

	void printfMatrix(XMFLOAT4X4 m, FILE *fp);
}
#include "Common\d3dApp.h"

namespace MathUntil
{
	XMVECTOR axisAngleToQuaternion(XMFLOAT3 axis, float angle);

	XMFLOAT3 lerpFloat3(XMFLOAT3 left, XMFLOAT3 right, float v1, float v2, float v3);

	float lerp(float left, float right, float v1, float v2, float v3);
}
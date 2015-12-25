#include "Common\d3dApp.h"
#include "MathHelp.h"

namespace MathUntil
{
	XMVECTOR axisAngleToQuaternion(XMFLOAT3 axis, float angle)
	{
		XMVECTOR vAxis = XMLoadFloat3(&axis);
		return XMQuaternionRotationAxis(vAxis, angle);
	}

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
}
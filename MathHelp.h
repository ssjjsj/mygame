#include "Common\d3dApp.h"

namespace MathUntil
{
	XMFLOAT3 lerpFloat3(XMFLOAT3 left, XMFLOAT3 right, float v1, float v2, float v3);

	float lerp(float left, float right, float v1, float v2, float v3);

	XMFLOAT4X4 GetTransformMatrix(XMFLOAT3 tranlate, XMFLOAT3 axis, float angle);
}
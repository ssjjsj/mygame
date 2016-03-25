#ifndef LIGHT_H
#define LIGHT_H
#include "MathHelp.h"

class Light
{
	enum LightType
	{
		Direct,
		Point,
		Spot,
	};

public:
	XMFLOAT3 pos;
	//XMFLOAT3 direction;
	XMFLOAT3 ambient;
	XMFLOAT3 diffuse;
	XMFLOAT3 specular;
	float k0, k1, k2;
};

#endif
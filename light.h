#ifndef LIGHT_H
#define LIGHT_H
#include "MathHelp.h"
#include "Mesh.h"
#include <vector>
#include "renderAble.h"
using namespace std;

class Light
{
public:
	Light();
	~Light();
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
	XMFLOAT3 k;

private:
	Mesh *cube;
private:
	void createBox();
public:
	vector<RenderAble*>& getRenderAble();
};

#endif
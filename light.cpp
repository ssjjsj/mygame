#include "light.h"

Light::Light()
{
	cube = NULL;
	createBox();
}

Light::~Light()
{
	if (cube != NULL)
		delete cube;
}

void Light::createBox()
{
	cube = new Mesh("cube.obj");
	cube->setMaterial("aabb.material.xml");
}


vector<RenderAble*>& Light::getRenderAble()
{
	XMMATRIX m1 = XMMatrixTranslation(pos.x, pos.y, pos.z);
	XMMATRIX m2 = XMMatrixScaling(10, 10, 10);
	vector<RenderAble*>& list = cube->getRenderAble();
	for (int i = 0; i < list.size(); i++)
	{
		list[i]->lighted = false;
		XMStoreFloat4x4(&list[i]->localMatrix, m2*m1);
	}

	return list;
}
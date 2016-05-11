#include "gameobject.h"

GameObject::GameObject()
{
	pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
}


GameObject::~GameObject()
{

}

void GameObject::AttachBehaviour(Behaviour *obj)
{
	behaviours.push_back(obj);
	obj->gameObject = this;
}

void GameObject::Update()
{
	for (int i = 0; i < behaviours.size(); i++)
	{
		Behaviour *b = behaviours[i];
		b->Update();
	}
}


void GameObject::SetPos(XMFLOAT3 pos)
{
	dirty = true;
	this->pos = pos;
}

void GameObject::SetScale(XMFLOAT3 scale)
{
	dirty = true;
	this->scale = scale;
}

XMFLOAT4X4 GameObject::WorldMatrix()
{
	if (dirty)
	{
		XMMATRIX m = XMMatrixTranslation(pos.x, pos.y, pos.z);
		XMStoreFloat4x4(&worldMatrix, m);
	}

	return worldMatrix;
}



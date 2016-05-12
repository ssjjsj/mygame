#include "gameobject.h"

GameObject::GameObject()
{
	pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(1.0, 1.0, 1.0);
	angleY = 0.0f;
	angleX = 0.0f;
}


GameObject::~GameObject()
{

}

void GameObject::AttachBehaviour(Behaviour *obj)
{
	behaviours.push_back(obj);
	obj->gameObject = this;
}

void GameObject::Update(float deltaTime)
{
	for (int i = 0; i < behaviours.size(); i++)
	{
		Behaviour *b = behaviours[i];
		b->Update(deltaTime);
	}
}


void GameObject::SetPos(XMFLOAT3 pos)
{
	dirty = true;
	this->pos = pos;
}

void GameObject::SetScale(float x, float y, float z)
{
	dirty = true;
	this->scale = XMFLOAT3(x, y, z);
}

void GameObject::RotateY(float angle)
{
	dirty = true;
	this->angleY = angle;
}

void GameObject::RotateX(float angle)
{
	angleX = angle;
}

XMFLOAT4X4 GameObject::WorldMatrix()
{
	if (dirty)
	{
		XMMATRIX m3 = XMMatrixRotationY(angleY);
		XMMATRIX m4 = XMMatrixRotationX(angleX);
		XMMATRIX m1 = XMMatrixTranslation(pos.x, pos.y, pos.z);
		XMMATRIX m2 = XMMatrixScaling(scale.x, scale.y, scale.z);
		
		XMStoreFloat4x4(&worldMatrix, m4*m3*m2*m1);
	}

	return worldMatrix;
}



#include "gameobject.h"

GameObject::GameObject()
{

}


GameObject::~GameObject()
{

}

void GameObject::AttachBehaviour(Behaviour *obj)
{
	behaviours.push_back(obj);
}

void GameObject::Update()
{
	for (int i = 0; i < behaviours.size(); i++)
	{
		Behaviour *b = behaviours[i];
		b->Update();
	}
}


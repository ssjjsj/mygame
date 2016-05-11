#ifndef GameObject_H
#define  GameObject_H

#include "MathHelp.h"
#include "behaviour.h"
#include <vector>
using namespace std;

struct AABB
{
	XMFLOAT4 min;
	XMFLOAT4 max;
};

class GameObject
{
public:
	GameObject();
	~GameObject();

public:
	
	AABB aabb;

	void AttachBehaviour(Behaviour *obj);
	void Update();

	void SetPos(XMFLOAT3 pos);
	void SetScale(XMFLOAT3 scale);
	//void SetRotation();
	XMFLOAT4X4 WorldMatrix();

private:
	vector<Behaviour*> behaviours;
	bool dirty;
	XMFLOAT3 pos;
	XMFLOAT3 scale;
	XMFLOAT4X4 worldMatrix;
};

#endif // !GameObject_H

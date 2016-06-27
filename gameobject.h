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
	void Update(float deltaTime);

	XMFLOAT3 GetPos();
	void SetPos(XMFLOAT3 pos);
	void SetScale(float x, float y, float z);
	void RotateY(float angle);
	void RotateX(float angle);
	XMFLOAT4X4 WorldMatrix();

private:
	vector<Behaviour*> behaviours;
	bool dirty;
	XMFLOAT3 pos;
	XMFLOAT3 scale;
	XMFLOAT4X4 worldMatrix;
	float angleY;
	float angleX;
};

#endif // !GameObject_H

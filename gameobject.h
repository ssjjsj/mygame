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
	XMFLOAT4X4 worldMatrix;
	AABB aabb;

	void AttachBehaviour(Behaviour *obj);
	void Update();

private:
	vector<Behaviour*> behaviours;
};

#endif // !GameObject_H

#ifndef NODE_H
#define NODE_H
#include <vector>
#include "MathHelp.h"
using namespace std;

class Node
{
public:
	Node();
public:
	XMFLOAT4 loaclQuaternion;
	XMFLOAT3 localTranslate;
	XMFLOAT4 globalQuaternion;
	XMFLOAT3 globalTranslate;

private:
	Node *parent;
	vector<Node*> children;

public:
	void update();
};
#endif
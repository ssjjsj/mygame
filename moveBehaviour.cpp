#include "moveBehaviour.h"
#include "worldMap.h"
#include "Mesh.h"

MoveBehaviour::MoveBehaviour()
{
	totalDelta = 0.0f;
}


MoveBehaviour::~MoveBehaviour()
{

}


void MoveBehaviour::clearPath()
{
	indexPath.clear();
}

void MoveBehaviour::moveTo(int x, int y)
{
	clearPath();
	indexPath = WorldMap::Instance().move(curX, curY, x, y);

	((Mesh*)gameObject)->playAnimation("RunBase");
	((Mesh*)gameObject)->playAnimation("RunTop");
}


void MoveBehaviour::SetPos(int x, int y)
{
	curX = x;
	curY = y;
}


void MoveBehaviour::Update(float delta)
{
	totalDelta += delta;
	if (totalDelta < 0.05f)
		return;
	totalDelta = 0.0f;

	if (indexPath.size() > 0)
	{
		PathNode node = indexPath[0];
		indexPath.erase(indexPath.cbegin());

		changeDirection(node.x, node.y);
		curX = node.x;
		curY = node.y;

		XMFLOAT3 pos = WorldMap::Instance().ToWorldPos(curX, curY);
		gameObject->SetPos(pos);
	}
	else
	{
		((Mesh*)gameObject)->stopAnimation("RunBase");
		((Mesh*)gameObject)->stopAnimation("RunTop");
		((Mesh*)gameObject)->playAnimation("IdleBase");
		((Mesh*)gameObject)->playAnimation("IdleTop");
	}
}


void MoveBehaviour::changeDirection(int x, int y)
{
	if (curX > x && y == curY)
		gameObject->RotateY(3.14f / 2);

	if (curX == x && y < curY)
		gameObject->RotateY(0.0f);

	if (curX<x && y == curY)
		gameObject->RotateY(3.14 * 3/2);

	if (curX == x && y>curY)
		gameObject->RotateY(3.14f);
}

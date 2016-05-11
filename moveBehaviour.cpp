#include "moveBehaviour.h"
#include "worldMap.h"

MoveBehaviour::MoveBehaviour()
{

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
}


void MoveBehaviour::Update()
{
	if (indexPath.size() > 0)
	{
		PathNode node = indexPath[indexPath.size() - 1];
		indexPath.pop_back();
		curX = node.x;
		curY = node.y;

		XMFLOAT3 pos = WorldMap::Instance().ToWorldPos(curX, curY);
		gameObject->SetPos(pos);
	}
}

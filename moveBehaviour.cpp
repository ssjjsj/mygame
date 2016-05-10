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
	indexPath = WorldMap::Instance().move(curX, curY, x, y);
}

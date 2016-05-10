#include "worldMap.h"
#include "MathHelp.h"

WorldMap::WorldMap()
{
}

WorldMap::~WorldMap()
{
}

void WorldMap::createMap(XMFLOAT2 startPoint, float length, int width, int height)
{
	this->startPoint = startPoint;
	this->width = width;
	this->height = height;
	this->lenght = length;
}

void WorldMap::createStaticBlocks()
{
	for (int i = 0; i < staticGameObjects.size(); i++)
	{
		GameObject obj = staticGameObjects[i];
		XMMATRIX m = XMLoadFloat4x4(&obj.worldMatrix);
		XMVECTOR min = XMVector4Transform(XMLoadFloat4(&obj.aabb.min), m);
		XMVECTOR max = XMVector4Transform(XMLoadFloat4(&obj.aabb.max), m);

		XMFLOAT4  fMin, fMax;
		XMStoreFloat4(&fMin, min);
		XMStoreFloat4(&fMax, max);

		int index1 = worldPosToIndex(fMin);
		int index2 = worldPosToIndex(fMax);

		int startIndex = index1;
		int endIndex = index2;

		if (index2 < index1)
		{
			startIndex = index2;
			endIndex = index1;
		}

		for (int i = startIndex; i <= endIndex; i++)
		{
			staticBlocks.push_back(i);
		}
	}
}


int WorldMap::worldPosToIndex(XMFLOAT4 &pos)
{
	int rowIndex = (pos.x - startPoint.x) / lenght;
	int colIndex = (pos.y - startPoint.y) / lenght;

	return colIndex*width + rowIndex;
}

vector<PathNode> WorldMap::findPath(int startIndex, int endIndex)
{
	int curIndex = startIndex;
	vector<int> roundIndex = getRoundIndex(curIndex);
	vector<int> bfsQueue;
	for (int i = 0; i < roundIndex.size(); i++)
		bfsQueue.push_back(roundIndex[i]);

	vector<int> path;
	path.push_back(startIndex);

	while (bfsQueue.size() != 0 && curIndex != endIndex)
	{
		if (roundIndex.size() == 0)
		{
			int lastNode = path[path.size() - 1];
			path.pop_back();
			if (lastNode == startIndex)
				break;
		}
		int nextNode = bfsQueue[bfsQueue.size() - 1];
		bfsQueue.pop_back();
		path.push_back(nextNode);
		roundIndex = getRoundIndex(nextNode);
	}

	vector<PathNode> result;

	for (int i = 0; i < path.size(); i++)
	{
		int index = path[i];
		PathNode node;
		node.x = path[i] % width;
		node.y = path[j] / width;
		result.push_back(node);
	}

	return result;
}

vector<int> WorldMap::getRoundIndex(int index)
{
	vector<int> result;
	int row = index%width;
	int col = index / width;

	if (row != 0)
		result.push_back(index - 1);
	else if (row != width - 1)
		result.push_back(index + 1);

	if (col != 0)
		result.push_back(index - width);
	else if (col != height)
		result.push_back(index + width);

	return result;
}


vector<PathNode> WorldMap::move(int startX, int startY, int endX, int endY)
{
	int startIndex = startX + startY*width;
	int endIndex = endX + endY*width;

	return findPath(startIndex, endIndex);
}

XMFLOAT3 WorldMap::ToWorldPos(int x, int y)
{
	XMFLOAT3 pos;
	pos.x = startPoint.x + x*lenght;
	pos.y = startPoint.y + y*lenght;
	pos.z = startPoint.z;

	return pos;
}



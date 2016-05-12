#include "worldMap.h"
#include "MathHelp.h"

WorldMap::WorldMap()
{
}

WorldMap::~WorldMap()
{
}

void WorldMap::createMap(XMFLOAT3 startPoint, float length, int width, int height)
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
		XMMATRIX m = XMLoadFloat4x4(&obj.WorldMatrix());
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


	vector<int> path;
	path.push_back(startIndex);

	vector<int> roundIndex = getRoundIndex(curIndex);

	while (curIndex != endIndex)
	{
		if (roundIndex.size() == 0)
			break;
		else
		{
			int min = abs(endIndex - roundIndex[0]);
			int nextNode = roundIndex[0];
			for (int i = 0; i < roundIndex.size(); i++)
			{
				int distance = abs(endIndex - roundIndex[i]);
				if (distance < min)
				{
					min = distance;
					nextNode = roundIndex[i];
				}
			}

			curIndex = nextNode;
			path.push_back(nextNode);
			roundIndex = getRoundIndex(nextNode);
		}
	}

	vector<PathNode> result;

	if (curIndex == endIndex)
	{
		for (int i = 0; i < path.size(); i++)
		{
			int index = path[i];
			PathNode node;
			node.x = path[i] % width;
			node.y = path[i] / width;
			result.push_back(node);
		}
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

	if (row != width - 1)
		result.push_back(index + 1);

	if (col != 0)
		result.push_back(index - width);
	
	if (col != height)
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
	pos.z = startPoint.z + y*lenght;
	pos.y = 10.0;

	return pos;
}



#ifndef WorldMap_H
#define  WorldMap_H
#include <vector>
#include "MathHelp.h"
#include "gameobject.h"
#include "singleTon.h"
using namespace std;

struct PathNode
{
	int x;
	int y;
};

class WorldMap : public Singleton<WorldMap>
{
public:
	WorldMap();
	~WorldMap();

private:
	int width;
	int height;
	float lenght;
	XMFLOAT3 startPoint;
	vector<int> staticBlocks;
	vector<int> dynamicBlocks;
	vector<GameObject> staticGameObjects;

public:
	void createMap(XMFLOAT3 startPoint, float length, int width, int height);
	void createStaticBlocks();
	vector<int> getRoundIndex(int index);
	vector<PathNode> move(int startX, int startY, int endX, int endY);
	XMFLOAT3 ToWorldPos(int x, int y);

private:
	int worldPosToIndex(XMFLOAT4 &pos);
	vector<PathNode> findPath(int startIndex, int endIndex);
};
#endif
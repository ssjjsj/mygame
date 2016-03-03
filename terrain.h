#ifndef TERRAIN_H
#include <string>
#include <vector>
#include "renderAble.h"
using namespace std;

class Terrain
{
	struct HeightData
	{
		vector<char> data;
		int xSize;
		int zSize;
	};

public:
	Terrain() {};
	~Terrain() {};

public:
	char getHeight(int x, int z);
	void setHeight(int x, int z, char height);
	void generateRenderAbles();
	vector<RenderAble*>& getRenderAbles() { return renderAbleList; }
	void loadData(string name);

private:
	int scale;
	HeightData heightData;
	int width;
	vector<RenderAble*> renderAbleList;
};

#endif // !TERRAIN_H

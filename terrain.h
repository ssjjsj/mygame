#ifndef TERRAIN_H
#include <string>
#include <vector>
#include "renderAble.h"
using namespace std;

class Terrain
{
	struct QuadNode
	{
		std::pair<int, int> centerPos;
		bool isSubdivided;
		float length;
		vector<QuadNode*> subNodes;
	};

	struct HeightData
	{
		vector<float> data;
		int xSize;
		int zSize;
	};

public:
	Terrain();
	~Terrain();

public:
	char getHeight(int x, int z);
	void setHeight(int x, int z, char height);
	void generateRenderAbles();
	vector<RenderAble*>& getRenderAbles() { return renderAbleList; }
	void loadData(string name);
	void fractal();

private:
	void initQuadTree();
	void createQuadNode(float length, QuadNode *parentNode);
	void destoryQuadTree();
	void destoryQuadNode(QuadNode *q);
	void reSetNode(QuadNode *q);
	void reBuildQuadTree();
	void renderQuadNode(QuadNode *node);
	void generateRenderAblesOnQuadTree();
	void generateRenderAblesOnQuad(QuadNode *node);

private:
	float scale;
	HeightData heightData;
	vector<RenderAble*> renderAbleList;
	QuadNode *rootNode;
	MyVertex::ModelData modelData;
};

#endif // !TERRAIN_H

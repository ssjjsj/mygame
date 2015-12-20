#ifndef VERTEX_H
#define VERTEX_H
#include "d3dApp.h"
#include <vector>
using namespace std;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Nor;
	XMFLOAT4 Color;
	XMFLOAT2 UV;
};


struct ModelData
{
	vector<Vertex> vertexs;
	vector<vector<int>> indexs;
	string textureName;
};
#endif
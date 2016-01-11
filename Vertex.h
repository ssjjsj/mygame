#ifndef VERTEX_H
#define VERTEX_H
#include "d3dApp.h"
#include <vector>
#include "BoneVertexAssignment.h"
using namespace std;

namespace MyVertex
{
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
		vector<UINT> indexs;
		string materialName;
		vector<BoneVertexAssignment> boneVertexAssigns;
	};
};
#endif
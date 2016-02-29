#ifndef VERTEX_H
#define VERTEX_H
#include "MathHelp.h"
#include <vector>
#include "BoneVertexAssignment.h"
using namespace std;

namespace MyVertex
{
	struct Vertex
	{
		XMFLOAT4 Pos;
		XMFLOAT3 Nor;
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
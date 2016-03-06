#ifndef VERTEX_H
#define VERTEX_H
#include "MathHelp.h"
#include <vector>
#include "BoneVertexAssignment.h"
using namespace std;

namespace MyVertex
{
	enum VertexType
	{
		P_N_UV = 0,
		P_C,
	};
	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Nor;
		XMFLOAT2 UV;
	};

	struct Vertex_P4_C
	{
		XMFLOAT4 Pos;
		XMFLOAT4 Color;
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
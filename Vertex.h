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
		XMFLOAT4 boneIndexs;
		XMFLOAT4 weight;

		Vertex()
		{
			Pos.x = 0.0f;
			Pos.y = 0.0f;
			Pos.z = 0.0f;
			Nor.x = 0.0f;
			Nor.y = 0.0f;
			Nor.z = 0.0f;
			UV.x = 0.0f;
			UV.y = 0.0f;
			weight.x = 0.0f;
			weight.y = 0.0f;
			weight.z = 0.0f;
			weight.w = 0.0f;
			boneIndexs.x = 0.0f;
			boneIndexs.y = 0.0f;
			boneIndexs.z = 0.0f;
			boneIndexs.w = 0.0f;
		}
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
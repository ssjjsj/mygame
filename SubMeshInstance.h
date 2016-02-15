#ifndef SUBMESHINSTANCE_H
#define SUBMESHINSTANCE_H
//#include "Common\d3dUtil.h"
#include <D3D11.h>
#include "Vertex.h"
#include <vector>
using namespace std;

class SubMeshInstance
{
public:
	SubMeshInstance();
	~SubMeshInstance();
public:
	void SetVertexData(vector<MyVertex::Vertex> vertexs, ID3D11Device* device);
	void SetIndexData(vector<int> indexs, ID3D11Device* device);
	void Draw();
private:
	ID3D11Buffer* vb;
	ID3D11Buffer* ib;
};

#endif
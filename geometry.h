#ifndef SUBMESHINSTANCE_H
#define SUBMESHINSTANCE_H
//#include "Common\d3dUtil.h"
#include <D3D11.h>
#include "Vertex.h"
#include <vector>
#include "shaderResource.h"
using namespace std;


enum VertexType
{
	PNCT = 0,
	PC = 1,
	PT = 2,
};

class Geometry
{
public:
	Geometry();
	~Geometry();
public:
	void setLayout(SourceCode &code);
	void setVertexData(vector<MyVertex::Vertex> &vertexs);
	void updateVertexData(vector<MyVertex::Vertex> &vertexs);
	void Geometry::updateIndexData(vector<UINT> &indices);
	void setIndexData(vector<UINT> &indexs);
	ID3D11Buffer* getVertexBuffer() { return vb; }
	ID3D11Buffer* getIndexBuffer() { return ib; }
	UINT getIndexCount() { return tcount; }
	MyVertex::VertexType getVertexType() { return vertexType; }
	static int getVertexSize(MyVertex::VertexType type);
private:
	ID3D11Buffer* vb;
	ID3D11Buffer* ib;
	UINT tcount;
	MyVertex::VertexType vertexType;
};

#endif
#include "SubMeshInstance.h"

SubMeshInstance::SubMeshInstance()
{

}

SubMeshInstance::~SubMeshInstance()
{
	if (vb != NULL)
		vb->Release();
	if (ib != NULL)
		ib->Release();
}



void SubMeshInstance::SetVertexData(vector<MyVertex::Vertex> vertices, ID3D11Device* device)
{
	UINT vcount = vertices.size();
	ID3D11Buffer *VB;
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(MyVertex::Vertex) * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(device->CreateBuffer(&vbd, &vinitData, &vb));
}


void SubMeshInstance::SetIndexData(vector<int> indices, ID3D11Device* device)
{
	UINT tcount = indices.size();
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * tcount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(device->CreateBuffer(&ibd, &iinitData, &ib));
}



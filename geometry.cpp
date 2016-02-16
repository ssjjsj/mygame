#include "geometry.h"
#include "global.h"
#include "shaderResource.h"

Geometry::Geometry()
{

}

Geometry::~Geometry()
{
	if (vb != NULL)
		vb->Release();
	if (ib != NULL)
		ib->Release();
}



void Geometry::SetVertexData(vector<MyVertex::Vertex> vertices, ID3D11Device* device)
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


void Geometry::SetIndexData(vector<int> indices, ID3D11Device* device)
{
	tcount = indices.size();
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


void Geometry::setLayout(SourceCode &code)
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ID3D11VertexShader *shader;
	gRender->Device()->d3dDevice->CreateInputLayout(vertexDesc, 4, code.data,
		code.length, &inputLayout);
}


int Geometry::getVertexSize(VertexType type)
{
	int size = 0; 

	size = sizeof(MyVertex::Vertex);

	return size;
}


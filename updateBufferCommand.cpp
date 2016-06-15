#include "updateBufferCommand.h"
#include <D3D11.h>
#include "Render.h"
#include "global.h"

UpdateBufferCommand::UpdateBufferCommand()
	: buffer(NULL), slot(0), isEnable(false)
{

}

void UpdateBufferCommand::bindToVs()
{
	ID3D11DeviceContext* immediateContext = gRender->Device()->immediateContext;
	immediateContext->VSSetConstantBuffers(slot, 1, &buffer);
}

void UpdateBufferCommand::bindToPs()
{
	ID3D11DeviceContext* immediateContext = gRender->Device()->immediateContext;
	immediateContext->PSSetConstantBuffers(slot, 1, &buffer);
}

UpdateBufferCommand::~UpdateBufferCommand()
{
	if (buffer != NULL)
		buffer->Release();
}

//matrix buffer
void UpdateMatrixBufferCommand::update()
{
	ID3D11DeviceContext* immediateContext = gRender->Device()->immediateContext;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	immediateContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	XMFLOAT4X4* dataPtr = (XMFLOAT4X4*)mappedResource.pData;
	memcpy(dataPtr, &data, sizeof(XMFLOAT4X4));
	immediateContext->Unmap(buffer, 0);
	immediateContext->VSSetConstantBuffers(slot, 1, &buffer);
}

void UpdateMatrixBufferCommand::updateData(XMFLOAT4X4 data)
{
	this->data = data;
}


void UpdateMatrixBufferCommand::init()
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(D3D11_BUFFER_DESC));
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	gRender->Device()->d3dDevice->CreateBuffer(&matrixBufferDesc, NULL, &buffer);
}

//light buffer
void UpdateLightBufferCommand::init()
{
	D3D11_BUFFER_DESC lightBufferDesc;
	ZeroMemory(&lightBufferDesc, sizeof(D3D11_BUFFER_DESC));
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightData);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	gRender->Device()->d3dDevice->CreateBuffer(&lightBufferDesc, NULL, &buffer);
}

void UpdateLightBufferCommand::update()
{
	ID3D11DeviceContext* immediateContext = gRender->Device()->immediateContext;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	gRender->Device()->immediateContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	LightData* dataPtr = (LightData*)mappedResource.pData;
	*dataPtr = data;
	immediateContext->Unmap(buffer, 0);
	immediateContext->VSSetConstantBuffers(slot, 1, &buffer);
}


void UpdateLightBufferCommand::updateLightData(Light *l)
{
	data.pos = l->pos;
	data.ambient = l->ambient;
	data.diffuse = l->diffuse;
	data.specular = l->specular;
	data.range = l->range;
}



//surface buffer
void UpdateSurfaceBufferCommand::init()
{
	D3D11_BUFFER_DESC surfaceBufferDesc;
	ZeroMemory(&surfaceBufferDesc, sizeof(D3D11_BUFFER_DESC));
	surfaceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	surfaceBufferDesc.ByteWidth = sizeof(SurfaceData);
	surfaceBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	surfaceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	gRender->Device()->d3dDevice->CreateBuffer(&surfaceBufferDesc, NULL, &buffer);
}


void UpdateSurfaceBufferCommand::updateSurfaceData(Material *m)
{
	data.ambient = m->getAmbient();
	//data.diffuse = m->getDiffuse();
	data.diffuse = XMFLOAT3(1.0f, 1.0f, 1.0f);
	//XMFLOAT3 specular = m->getSpecular();
	data.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 20.0);
}


void UpdateSurfaceBufferCommand::update()
{
	ID3D11DeviceContext* immediateContext = gRender->Device()->immediateContext;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	gRender->Device()->immediateContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	SurfaceData* dataPtr = (SurfaceData*)mappedResource.pData;
	*dataPtr = data;
	immediateContext->Unmap(buffer, 0);
	immediateContext->VSSetConstantBuffers(slot, 1, &buffer);
}


void UpdateSkinMatrixsBufferCommand::update()
{
	ID3D11DeviceContext* immediateContext = gRender->Device()->immediateContext;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	gRender->Device()->immediateContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	XMFLOAT4X4* dataPtr = (XMFLOAT4X4*)mappedResource.pData;
	for (int i = 0; i < data.size(); i++)
	{
		dataPtr[i] = data[i];
	}
	immediateContext->Unmap(buffer, 0);
	immediateContext->VSSetConstantBuffers(slot, 1, &buffer);
}


void UpdateSkinMatrixsBufferCommand::updateMatrix(vector<XMFLOAT4X4> &data)
{
	this->data = data;
}


void UpdateSkinMatrixsBufferCommand::init()
{
	D3D11_BUFFER_DESC skinMatrixsBufferDesc;
	ZeroMemory(&skinMatrixsBufferDesc, sizeof(D3D11_BUFFER_DESC));
	skinMatrixsBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	skinMatrixsBufferDesc.ByteWidth = sizeof(XMFLOAT4X4)*maxBones;
	skinMatrixsBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	skinMatrixsBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	gRender->Device()->d3dDevice->CreateBuffer(&skinMatrixsBufferDesc, NULL, &buffer);
}
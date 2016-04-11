#include "updateBufferCommand.h"

UpdateBufferCommand::UpdateBufferCommand()
	: buffer(NULL), slot(0), isEnable(false)
{

}

UpdateBufferCommand::UpdateBufferCommand()
{
	if (buffer != NULL)
		buffer->Release();
}

//matrix buffer
void UpdateMatrixBufferCommand::update()
{
	ID3D11DeviceContext* immediateContext = gRender->Device->immediateContext;

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
	gRender->Device->d3dDevice->CreateBuffer(&matrixBufferDesc, NULL, &buffer);
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
	gRender->Device->d3dDevice->CreateBuffer(&lightBufferDesc, NULL, &buffer);
}

void UpdateLightBufferCommand::update()
{
	ID3D11DeviceContext* immediateContext = gRender->Device->immediateContext;

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
}



//surface buffer
void UpdateSurfaceBufferCommand::updateSurfaceData(Material *m)
{
	data.ambient = m->getAmbient();
	data.diffuse = m->getDiffuse();
	XMFLOAT3 specular = m->getSpecular();
	data.specular = XMFLOAT4(specular.x, specular.y, specular.z, 1.0);
}


void UpdateSurfaceBufferCommand::update()
{
	ID3D11DeviceContext* immediateContext = gRender->Device->immediateContext;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	gRender->Device()->immediateContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	SurfaceData* dataPtr = (SurfaceData*)mappedResource.pData;
	*dataPtr = data;
	immediateContext->Unmap(buffer, 0);
	immediateContext->VSSetConstantBuffers(slot, 1, &buffer);
}
#ifndef UPDATEBUFFERCOMMAND_H
#define UPDATEBUFFERCOMMAND_H

#include "MathHelp.h"
#include "material.h"
#include "light.h"

class UpdateBufferCommand
{
public:
	UpdateBufferCommand();
	virtual ~UpdateBufferCommand();

	virtual void init() {};
	virtual  void update() {};
	void enable() { isEnable = true; };
	void disable() { isEnable = false; };
	bool IsEnable() { return isEnable; }
	void setSlot(int slot) { this->slot = slot; };
	void bindToVs();
	void bindToPs();
protected:
	int slot;
	ID3D11Buffer *buffer;
private:
	bool isEnable;
};




class UpdateMatrixBufferCommand : public UpdateBufferCommand
{
public:
	UpdateMatrixBufferCommand(){};
	~UpdateMatrixBufferCommand(){};
	void init();
	void update();
	void updateData(XMFLOAT4X4 data);
private:
	XMFLOAT4X4 data;
};

class UpdateLightBufferCommand : public UpdateBufferCommand
{
	struct LightData
	{
		XMFLOAT3 pos;
		int pad1;
		//XMFLOAT3 direction;
		XMFLOAT3 ambient;
		int pad2;
		XMFLOAT3 diffuse;
		int pad3;
		XMFLOAT3 specular;
		float range;
	};
public:
	UpdateLightBufferCommand(){};
	~UpdateLightBufferCommand(){};
	void init();
	void update();
	void updateLightData(Light *l);
private:
	LightData data;
};

class UpdateSurfaceBufferCommand : public UpdateBufferCommand
{
	struct SurfaceData
	{
		XMFLOAT3 ambient;
		int pad1;
		XMFLOAT3 diffuse;
		int pad2;
		XMFLOAT4 specular;
	};
public:
	UpdateSurfaceBufferCommand(){};
	~UpdateSurfaceBufferCommand(){};
	void init();
	void update();
	void updateSurfaceData(Material *m);

private:
	SurfaceData data;
};


class UpdateSkinMatrixsBufferCommand : public UpdateBufferCommand
{
public:
	UpdateSkinMatrixsBufferCommand(){};
	~UpdateSkinMatrixsBufferCommand(){};
	void init();
	void update();
	void updateMatrix(vector<XMFLOAT4X4> &data);
	const int maxBones = 128;

private:
	vector<XMFLOAT4X4> data;
};

#endif
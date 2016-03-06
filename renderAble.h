#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "geometry.h"
#include "material.h"

class RenderAble
{
public:
	RenderAble() { XMStoreFloat4x4(&localMatrix, XMMatrixIdentity()); };
	RenderAble(Geometry *g, Material *m);
	~RenderAble();
	XMFLOAT4X4 localMatrix;
public:
	Material* getMaterial() { return mat; };
	Geometry* getGeometry() { return geometry; };
private:
	Material *mat;
	Geometry *geometry;
};

#endif
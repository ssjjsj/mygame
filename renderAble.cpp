#include "renderAble.h"

RenderAble::RenderAble(Geometry *g, Material *m)
{
	mat = m;
	geometry = g;

	XMStoreFloat4x4(&localMatrix, XMMatrixIdentity());
}


RenderAble::~RenderAble()
{
	delete mat;
	delete geometry;
}
#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "geometry.h"
#include "material.h"

class RenderAble
{
public:
	RenderAble() {};
	RenderAble(Geometry *g, Material *m);
	~RenderAble();
public:
	Material* getMaterial() { return mat; }
	Geometry* getGeometry() { return geometry; }
private:
	Material *mat;
	Geometry *geometry;
};

#endif
#include "renderAble.h"

RenderAble::RenderAble(Geometry *g, Material *m)
{
	mat = m;
	geometry = g;
}


RenderAble::~RenderAble()
{

}
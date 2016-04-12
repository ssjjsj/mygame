#ifndef POSTEFFECT_H
#define POSTEFFECT_H
#include "renderAble.h"


class PostEffect
{
public:
	PostEffect(string MaterialName);
	~PostEffect();

	void Render();

	Material* getMaterial() { return obj->getMaterial(); }

private:
	RenderAble *obj;
};

#endif
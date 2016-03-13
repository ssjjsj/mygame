#ifndef SKYBOX_H
#define SKYBOX_H

#include "renderAble.h"

class SkyBox
{
public:
	SkyBox() {};
	~SkyBox() {};

	void createSkyBox();
	vector<RenderAble*>& getRenderAble() { return renderAbles; }
private:
	vector<RenderAble*> renderAbles;
};

#endif
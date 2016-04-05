#ifndef SKYBOX_H
#define SKYBOX_H

#include "renderAble.h"

class SkyBox
{
public:
	SkyBox() {};
	~SkyBox() 
	{
		for (int i = 0; i < renderAbles.size(); i++)
			delete renderAbles[i];
	};

	void createSkyBox();
	vector<RenderAble*>& getRenderAble() { return renderAbles; }
private:
	vector<RenderAble*> renderAbles;
};

#endif
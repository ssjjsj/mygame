#ifndef RENDER_H
#define RENDER_H

#include "RenderDevice.h"


struct RenderOperation
{
	enum VertexLayout
	{
		PNCT = 0,
		PC = 1,
		PT = 2,
	};
};

class Render
{
public:
	Render(RenderDevice *device);
	~Render();

private:
	RenderDevice *renderDevice;
	void init();
};
#endif
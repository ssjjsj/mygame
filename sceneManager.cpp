#include "sceneManager.h"
#include "global.h"
#include "Render.h"

SceneManager::SceneManager()
{
	terrain = NULL;
	skyBox = NULL;
}

SceneManager::~SceneManager()
{
	if (terrain != NULL)
		delete terrain;

	if (skyBox != NULL)
		delete skyBox;
}


void SceneManager::createTerrain()
{
	terrain = new Terrain();
	terrain->loadData("");
	terrain->generateRenderAbles();

	skyBox = new SkyBox;
	skyBox->createSkyBox();
}

void SceneManager::update(float delta)
{
	for (int i = 0; i < meshAry.size(); i++)
	{
		meshAry[i]->update(delta);
	}

	/*if (terrain)
		terrain->updateQuadTree();*/
}


void SceneManager::render()
{
	renderAbleAry.clear();

	vector<RenderAble*>& renderAbles = skyBox->getRenderAble();
	for (int i = 0; i < renderAbles.size(); i++)
	{
		RenderAble *obj = renderAbles[i];
		renderAbleAry.push_back(obj);
	}

	for (int i = 0; i < meshAry.size(); i++)
	{
		vector<RenderAble*>& renderAbles = meshAry[i]->getRenderAble();
		for (int j = 0; j < renderAbles.size(); j++)
		{
			renderAbleAry.push_back(renderAbles[j]);
		}
	}

	if (terrain != NULL)
	{
		vector<RenderAble*>& renderAbles = terrain->getRenderAbles();
		for (int j = 0; j < renderAbles.size(); j++)
		{
			renderAbleAry.push_back(renderAbles[j]);
		}
	}
	
	gRender->draw(renderAbleAry);
}
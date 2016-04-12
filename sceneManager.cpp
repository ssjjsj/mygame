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
	for (int i = 0; i < meshAry.size(); i++)
	{
		delete meshAry[i];
	}


	if (terrain != NULL)
		delete terrain;

	if (skyBox != NULL)
		delete skyBox;

	for (int i = 0; i < lights.size(); i++)
	{
		delete lights[i];
	}
}


void SceneManager::addLight(Light *light)
{
	lights.push_back(light);
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

	//if (terrain)
	//	terrain->updateQuadTree();
}


void SceneManager::render()
{
	renderAbleAry.clear();

	if (skyBox != NULL)
	{
		vector<RenderAble*>& renderAbles = skyBox->getRenderAble();
		for (int i = 0; i < renderAbles.size(); i++)
		{
			RenderAble *obj = renderAbles[i];
			renderAbleAry.push_back(obj);
		}
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

	//for (int i = 0; i < lights.size(); i++)
	//{
	//	vector<RenderAble*>& renderAbles = lights[i]->getRenderAble();
	//	for (int j = 0; j < renderAbles.size(); j++)
	//	{
	//		renderAbleAry.push_back(renderAbles[j]);
	//	}
	//}

	gRender->preDraw();
	if (lights.size() > 0)
	{
		gRender->draw(renderAbleAry, lights);
	}
	gRender->PostDraw();
}
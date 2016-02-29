#include "sceneManager.h"
#include "global.h"
#include "Render.h"

void SceneManager::update(float delta)
{
	for (int i = 0; i < meshAry.size(); i++)
	{
		meshAry[i]->update(delta);
	}
}


void SceneManager::render()
{
	renderAbleAry.clear();

	for (int i = 0; i < meshAry.size(); i++)
	{
		vector<RenderAble*>& renderAbles = meshAry[i]->getRenderAble();
		for (int j = 0; j < renderAbles.size(); j++)
		{
			renderAbleAry.push_back(renderAbles[j]);
		}
	}

	gRender->draw(renderAbleAry);
}
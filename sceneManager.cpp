#include "sceneManager.h"

void SceneManager::update(float delta)
{
	for (int i = 0; i < meshAry.size(); i++)
	{
		meshAry[i]->update(delta);
	}
}


void SceneManager::render()
{
	for (int i = 0; i < meshAry.size(); i++)
	{
		vector<RenderAble*>& renderAbles = meshAry[i]->getRenderAble();
		for (int j = 0; j < renderAbles.size(); j++)
		{
			renderAbleAry.push_back(renderAbles[j]);
		}
	}
}
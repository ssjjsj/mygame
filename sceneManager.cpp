#include "sceneManager.h"
#include "global.h"
#include "Render.h"
#include "worldMap.h"
#include "terrain.h"

SceneManager::SceneManager()
{
	terrain = NULL;
	skyBox = NULL;
	mainCamera = new Camera;
	lightCamera = new Camera;

	mainCamera->SetLens(3.14, (float)800 / (float)600, 1.0f, 1000.0f);
	lightCamera->SetLens(3.14, (float)800 / (float)600, 1.0f, 1000.0);
	
	XMVECTOR pos = XMVectorSet(0.0f, 150.0f, 200.0f, 1.0f);
	XMVECTOR target = XMVectorSet(200.0f, 0.0f, 200.0f, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	lightCamera->LookAt(pos, target, up);
	lightCamera->UpdateViewMatrix();


	renderAbleAry["terrain"] = vector<RenderAble*>();
	renderAbleAry["shadowMesh"] = vector<RenderAble*>();
	renderAbleAry["skyBox"] = vector<RenderAble*>();
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

	WorldMap::Instance().createMap(XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f, 512, 512);

	skyBox = new SkyBox;
	//skyBox->createSkyBox();
}

void SceneManager::update(float delta)
{
	for (int i = 0; i < meshAry.size(); i++)
	{
		meshAry[i]->Update(delta);
	}

	//if (terrain)
	//	terrain->updateQuadTree();
}


void SceneManager::render()
{
	renderAbleAry["terrain"].clear();
	renderAbleAry["shadowMesh"].clear();
	renderAbleAry["skyBox"].clear();

	gRender->SetCamera(mainCamera);
	if (skyBox != NULL)
	{
		vector<RenderAble*>& renderAbles = skyBox->getRenderAble();
		for (int i = 0; i < renderAbles.size(); i++)
		{
			RenderAble *obj = renderAbles[i];
			renderAbleAry["skyBox"].push_back(obj);
		}
	}

	if (terrain != NULL)
	{
		vector<RenderAble*>& renderAbles = terrain->getRenderAbles();
		for (int j = 0; j < renderAbles.size(); j++)
		{
			XMFLOAT4X4 data;
			XMStoreFloat4x4(&data, XMMatrixIdentity());
			renderAbles[j]->localMatrix = data;
			renderAbleAry["shadowMesh"].push_back(renderAbles[j]);
		}
	}

	for (int i = 0; i < meshAry.size(); i++)
	{
		vector<RenderAble*>& renderAbles = meshAry[i]->getRenderAble();
		for (int j = 0; j < renderAbles.size(); j++)
		{
			renderAbles[j]->localMatrix = meshAry[i]->WorldMatrix();
			renderAbleAry["shadowMesh"].push_back(renderAbles[j]);
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
	//gRender->draw(renderAbleAry["terrain"]);
	gRender->drawShadow(renderAbleAry["shadowMesh"], lightCamera, mainCamera);
	gRender->PostDraw();
}
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

using namespace std;
#include "Mesh.h"
#include "renderAble.h"
#include <vector>
#include "terrain.h"
#include "SkyBox.h"
#include "light.h"
#include "Camera.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

public:
	void update(float delat);
	void render();
	void addMesh(Mesh *m) { meshAry.push_back(m); }
	void createTerrain();
	void addLight(Light *l);

private:
	vector<Mesh*> meshAry;
	vector<RenderAble*> renderAbleAry;
	Terrain *terrain;
	SkyBox *skyBox;
	vector<Light*> lights;
	Camera *mainCamera;
	Camera *lightCamera;
};
#endif
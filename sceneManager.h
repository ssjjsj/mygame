#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

using namespace std;
#include "Mesh.h"
#include "renderAble.h"
#include <vector>
#include <map>
#include "SkyBox.h"
#include "light.h"
#include "Camera.h"
#include "singleTon.h"

class Terrain;

class SceneManager : public Singleton<SceneManager> 
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
	Camera * getMainCamera() { return mainCamera; }
	Camera * getLightCamera() { return lightCamera; }

private:
	vector<Mesh*> meshAry;
	map<string, vector<RenderAble*>> renderAbleAry;
	Terrain *terrain;
	SkyBox *skyBox;
	vector<Light*> lights;
	Camera *mainCamera;
	Camera *lightCamera;
};
#endif
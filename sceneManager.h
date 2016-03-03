#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

using namespace std;
#include "Mesh.h"
#include "renderAble.h"
#include <vector>
#include "terrain.h"

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

private:
	vector<Mesh*> meshAry;
	vector<RenderAble*> renderAbleAry;
	Terrain *terrain;
};
#endif
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

using namespace std;
#include "Mesh.h"
#include "renderAble.h"
#include <vector>

class SceneManager
{
public:
	SceneManager() {};
	~SceneManager() {};

public:
	void update(float delat);
	void render();
	void addMesh(Mesh *m) { meshAry.push_back(m); }

private:
	vector<Mesh*> meshAry;
	vector<RenderAble*> renderAbleAry;
};
#endif
#ifndef MESH_H
#define MESH_H
#include "OrgeMeshPaser.h"
#include <string>
#include <vector>
#include "Vertex.h"
#include "BoneVertexAssignment.h"
#include "Vertex.h"
#include "Aniamtion.h"
#include "renderAble.h"
#include "gameobject.h"
using namespace std;


class Matrial
{
public:
	string matrialName;
	LPCWSTR texName;
};

class Mesh : public GameObject
{
public:
	Mesh();
	Mesh(string fileName);
	~Mesh();
private:
	void loadFile(string fileName);

public:
	vector<RenderAble*>& getRenderAble() { return renderAbleList; }
	void Update(float deltaTime);
	void attachAnimation(string animationName);
	void playAnimation(string animationName);
	void stopAnimation(string animationName);
	bool IsPlayAnimation()
	{
		return curAnimations.size() != 0;
	}
	void setMaterial(string name);
private:
	void DrawSubMesh();
	void skin();

private:
	vector<MyVertex::ModelData> subMeshAry;
	vector<MyVertex::ModelData> skinedMeshAry;
	vector<Animation*> animations;
	vector<Animation*> curAnimations;
	vector<RenderAble*> renderAbleList;
	string MatName;
	Skeleton skeleton;
};

#endif

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
using namespace std;


class Matrial
{
public:
	string matrialName;
	LPCWSTR texName;
};

class Mesh
{
public:
	Mesh();
	Mesh(string fileName);
	~Mesh();
private:
	void loadFile(string fileName);

public:
	vector<RenderAble*>& getRenderAble() { return renderAbleList; }
	void update(float deltaTime);
	vector<MyVertex::ModelData>& getModelData();
	vector<MyVertex::ModelData> getSkeletonModelData();
	void playAnimation(string animationName);
	bool IsPlayAnimation()
	{
		return curAnimation != NULL;
	}
private:
	void DrawSubMesh();
	void skin();

private:
	vector<MyVertex::ModelData> subMeshAry;
	vector<MyVertex::ModelData> skinedMeshAry;
	vector<Animation*> animations;
	Animation *curAnimation;
	vector<RenderAble*> renderAbleList;
};

#endif

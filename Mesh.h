#ifndef MESH_H
#define MESH_H
#include "OrgeMeshPaser.h"
#include <string>
#include <vector>
#include "Vertex.h"
#include "BoneVertexAssignment.h"
#include "Vertex.h"
#include "Aniamtion.h"
using namespace std;

class Mesh
{
public:
	Mesh();
	Mesh(string fileName);
	~Mesh();
private:
	void loadFile(string fileName);

public:
	void Draw();
	void update(float deltaTime);
private:
	void DrawSubMesh();
	void skin();
	void playAnimation(string animationName);

private:
	vector<MyVertex::ModelData> subMeshAry;
	vector<MyVertex::ModelData> skinedMeshAry;
	vector<Animation*> animations;
	Animation *curAnimation;
};

#endif

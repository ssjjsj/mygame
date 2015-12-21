#ifndef MESH_H
#define MESH_H
#include "OrgeMeshPaser.h"
#include <string>
#include <vector>
#include "Vertex.h"
#include "BoneVertexAssignment.h"
#include "Vertex.h"
using namespace std;

class Mesh
{
public:
	Mesh();
	Mesh(string fileName);
private:
	void loadFile(string fileName);

public:
	void Draw();

private:
	vector<ModelData> subMeshAry;
};

#endif

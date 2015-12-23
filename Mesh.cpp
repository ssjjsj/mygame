#include "Mesh.h"

Mesh::Mesh(string filename)
{
	loadFile(filename);
}

void Mesh::loadFile(string filename)
{
	subMeshAry = OrgeMeshPaser::Instance().parseMesh(filename);
}


void DrawSubMesh()
{

}
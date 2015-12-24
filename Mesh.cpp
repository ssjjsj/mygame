#include "Mesh.h"
using namespace MyVertex;
#include <map>
using namespace std;

Mesh::Mesh(string filename)
{
	loadFile(filename);
}

Mesh::~Mesh()
{
	for (int i = 0; i < animations.size(); i++)
	{
		delete animations[i];
	}
}

void Mesh::loadFile(string filename)
{
	subMeshAry = OrgeMeshPaser::parseMesh(filename);
}

void Mesh::playAnimation(string animationName)
{
	Animation *animation = NULL;
	for (int i = 0; animations.size(); i++)
	{
		if (animations[i]->GetName() == animationName)
		{
			animation = animations[i];
		}
	}

	if (animation == NULL)
	{
		Animation *a = new Animation(animationName);
		animations.push_back(a);
		curAnimation = a;
	}
}

void Mesh::update(float deltaTime)
{
	if (curAnimation != NULL)
	{
		curAnimation->update(deltaTime);
		skin();
	}
}


void Mesh::skin()
{
	map<string, XMMATRIX> matrixMap;
	skinedMeshAry = subMeshAry;

	for (int indexSubMesh = 0; indexSubMesh < subMeshAry.size(); indexSubMesh++)
	{
		ModelData &modelData = subMeshAry[indexSubMesh];
		map<int, XMFLOAT3> posMap;
		for (int indexBoneAssign = 0; indexBoneAssign < modelData.boneVertexAssigns.size(); indexBoneAssign++)
		{
			BoneVertexAssignment &ass = modelData.boneVertexAssigns[indexBoneAssign];
			Vertex &v = subMeshAry[indexSubMesh].vertexs[ass.vertexIndex];
			Skeleton::Bone *bone = curAnimation->GetSkeleton()->GetBone(ass.boneIndex);
			XMVECTOR partV =	 XMVector3Transform(XMLoadFloat3(&v.Pos), matrixMap[bone->name]);
			XMFLOAT3 partPos;
			XMStoreFloat3(&partPos, partV);
			posMap[ass.vertexIndex].x += partPos.x;
			posMap[ass.vertexIndex].y += partPos.y;
			posMap[ass.vertexIndex].z += partPos.z;
		}

		for (map<int, XMFLOAT3>::iterator it = posMap.begin(); it != posMap.end(); it++)
		{
			skinedMeshAry[indexSubMesh].vertexs[it->first].Pos = it->second;
		}
	}
}


void DrawSubMesh()
{

}
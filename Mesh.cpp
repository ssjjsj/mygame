#include "Mesh.h"
using namespace MyVertex;
#include <map>
using namespace std;

Mesh::Mesh()
{

}

Mesh::Mesh(string filename)
{
	loadFile(filename);
	curAnimation = NULL;
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
	map<string, XMFLOAT4X4> matrixMap = curAnimation->GetPosMatrix();
	skinedMeshAry = subMeshAry;

	for (int indexSubMesh = 0; indexSubMesh < subMeshAry.size(); indexSubMesh++)
	{
		ModelData &modelData = subMeshAry[indexSubMesh];
		map<int, XMFLOAT3> posMap;
		for (int indexBoneAssign = 0; indexBoneAssign < modelData.boneVertexAssigns.size(); indexBoneAssign++)
		{
			BoneVertexAssignment &ass = modelData.boneVertexAssigns[indexBoneAssign];
			posMap[ass.vertexIndex] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
		for (int indexBoneAssign = 0; indexBoneAssign < modelData.boneVertexAssigns.size(); indexBoneAssign++)
		{
			BoneVertexAssignment &ass = modelData.boneVertexAssigns[indexBoneAssign];
			Vertex &v = subMeshAry[indexSubMesh].vertexs[ass.vertexIndex];
			Skeleton::Bone *bone = curAnimation->GetSkeleton()->GetBone(ass.boneIndex);

			XMMATRIX m = XMLoadFloat4x4(&bone->posMatrix);
			XMVECTOR posV = XMLoadFloat4(&XMFLOAT4(v.Pos.x, v.Pos.y, v.Pos.z, 1.0f));
			XMVECTOR partV =	 XMVector4Transform(posV, m);
			
			XMFLOAT4 partPos;
			XMStoreFloat4(&partPos, partV);

			XMFLOAT3 curPos = posMap[ass.vertexIndex];
			curPos.x += partPos.x*ass.weight;
			curPos.y += partPos.y*ass.weight;
			curPos.z += partPos.z*ass.weight;
			posMap[ass.vertexIndex] = curPos;

			int i = 0;
		}

		for (map<int, XMFLOAT3>::iterator it = posMap.begin(); it != posMap.end(); it++)
		{
			XMFLOAT3 pos = it->second;
			skinedMeshAry[indexSubMesh].vertexs[it->first].Pos = pos;
		}
	}
}


void DrawSubMesh()
{

}


vector<MyVertex::ModelData>& Mesh::getModelData()
{
	if (curAnimation != NULL)
		return skinedMeshAry;
	else
		return subMeshAry;
}


vector<MyVertex::ModelData> Mesh::getSkeletonModelData()
{
	vector < MyVertex::ModelData > modelAry;
	MyVertex::ModelData data;

	for (int i = 0; i < curAnimation->GetSkeleton()->GetBones().size(); i++)
	{
		Skeleton::Bone* b = curAnimation->GetSkeleton()->GetBone(i);
		XMFLOAT4 v = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		XMMATRIX posMatrix = XMLoadFloat4x4(&curAnimation->GetSkeleton()->matrixMap[b->name]);
		XMVECTOR vecto = XMVector4Transform(XMLoadFloat4(&v), posMatrix);
		XMFLOAT4 pos;
		XMStoreFloat4(&pos, vecto);
		MyVertex::Vertex vertex;
		vertex.Pos = XMFLOAT3(pos.x, pos.y, pos.z);
		data.vertexs.push_back(vertex);

		if (b->IsRootBone() || b->parent->IsRootBone() || b->parent->parent->IsRootBone() || true)
		{
			for (int j = 0; j < b->children.size(); j++)
			{
				Skeleton::Bone *c = b->children[j];
				vector<int> ary;
				ary.push_back(b->id);
				ary.push_back(c->id);
				data.indexs.push_back(ary);
			}
		}
	}

	modelAry.push_back(data);

	return modelAry;
}
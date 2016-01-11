#include "Mesh.h"
using namespace MyVertex;
#include <map>
#include "MathHelp.h"
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
		//skin();
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
			
			XMFLOAT4 partPos = MathUntil::vectorMupilyMatrix(XMFLOAT4(v.Pos.x, v.Pos.y, v.Pos.z, 1.0f), bone->posMatrix);

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
	if (curAnimation != NULL && false)
	{
		if (curAnimation->HasDataChanged())
			return skinedMeshAry;
		else
		{
			vector < MyVertex::ModelData > m;
			return m;
		}
	}
	else
		return subMeshAry;
}


vector<MyVertex::ModelData> Mesh::getSkeletonModelData()
{
	vector < MyVertex::ModelData > modelAry;
	MyVertex::ModelData data;

	if (!curAnimation->HasDataChanged())
		modelAry;

	for (int i = 0; i < curAnimation->GetSkeleton()->GetBones().size(); i++)
	{
		Skeleton::Bone* b = curAnimation->GetSkeleton()->GetBone(i);
		XMFLOAT4 v = XMFLOAT4(0.0f, 0.0f, 0.0f, 10.0f);

		MyVertex::Vertex vertex;
		vertex.Pos = XMFLOAT3(b->globalTranslate.x + v.x, b->globalTranslate.y + v.y, b->globalTranslate.z + v.z);
		data.vertexs.push_back(vertex);

		if (b->name == "Root")
			vertex.Color = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

		//if (b->name == "Waist")
		//	vertex.Color = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

		if (b->name == "Thigh.L")
			vertex.Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

		Skeleton::Bone *root = curAnimation->GetSkeleton()->GetBone("root");
		if (b == root || b->parent == root || true)
		{
			for (int j = 0; j < b->children.size(); j++)
			{
				Skeleton::Bone *c = b->children[j];
				data.indexs.push_back(b->id);
				data.indexs.push_back(c->id);
			}
		}
	}

	modelAry.push_back(data);

	return modelAry;
}
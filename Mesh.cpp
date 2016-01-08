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
	if (curAnimation != NULL)
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
		XMFLOAT4 v = XMFLOAT4(-5.0f, 0.0f, 1.0f, 1.0f);

		XMFLOAT3X3 rot3x3 = MathUntil::QuaternionToRotationMatrix(b->globalQuaternion);

		XMFLOAT4X4 posMatrix;
		posMatrix._11 = rot3x3._11;
		posMatrix._12 = rot3x3._12;
		posMatrix._13 = rot3x3._13;
		posMatrix._14 = b->globalTranslate.x;

		posMatrix._21 = rot3x3._21;
		posMatrix._22 = rot3x3._22;
		posMatrix._23 = rot3x3._23;
		posMatrix._24 = b->globalTranslate.y;

		posMatrix._31 = rot3x3._31;
		posMatrix._32 = rot3x3._32;
		posMatrix._33 = rot3x3._33;
		posMatrix._34 = b->globalTranslate.z;

		posMatrix._41 = 0.0f;
		posMatrix._42 = 0.0f;
		posMatrix._43 = 0.0f;
		posMatrix._44 = 0.0f;

		XMFLOAT4 pos = MathUntil::vectorMupilyMatrix(v, posMatrix);

		MyVertex::Vertex vertex;
		vertex.Pos = XMFLOAT3(pos.x, pos.y, pos.z);
		data.vertexs.push_back(vertex);

		Skeleton::Bone *root = curAnimation->GetSkeleton()->GetBone("root");
		if (b == root || b->parent == root || true)
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
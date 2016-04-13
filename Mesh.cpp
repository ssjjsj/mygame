#include "Mesh.h"
using namespace MyVertex;
#include <map>
#include "MathHelp.h"
#include "global.h"
#include "Render.h"
using namespace std;

Mesh::Mesh()
{

}

Mesh::Mesh(string filename)
{
	XMStoreFloat4x4(&localMatrix, XMMatrixIdentity());
	loadFile(filename);
	curAnimation = NULL;
}

Mesh::~Mesh()
{
	for (int i = 0; i < animations.size(); i++)
	{
		delete animations[i];
	}

	for (int i = 0; i < renderAbleList.size(); i++)
	{
		delete renderAbleList[i];
	}
}

void Mesh::loadFile(string filename)
{
	string format = filename.substr(filename.size() - 3, 3);
	if (format == "xml")
		subMeshAry = OrgeMeshPaser::parseMesh(filename);
	else if (format == "obj")
		subMeshAry = OrgeMeshPaser::parseObjMesh(filename);
}

void Mesh::setMaterial(string name) 
{ 
	MaterialRes res = MaterialRes(name);

	for (int i = 0; i < subMeshAry.size(); i++)
	{

		MyVertex::ModelData &model = subMeshAry[i];

		Geometry *g = new Geometry();
		g->setIndexData(model.indexs);
		g->setVertexData(model.vertexs);


		MaterialRes::MaterialData data = res.getMaterialData(model.materialName);
		Material *m = new Material(data);
		RenderAble *obj = new RenderAble(g, m);
		obj->lighted = lighted;
		obj->localMatrix = localMatrix;
		renderAbleList.push_back(obj);
	}
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

	for (int i = 0; i < skinedMeshAry.size(); i++)
	{
		ModelData &data = skinedMeshAry[i];
		RenderAble *obj = renderAbleList[i];

		obj->getGeometry()->updateVertexData(data.vertexs);
	}


	//for (int i = 0; i < renderAbleList.size(); i++)
	//{
	//	RenderAble *obj = renderAbleList[i];

	//	ModelData &model = subMeshAry[i];
	//	vector<Vertex> newVertexs;
	//	for (int j = 0; j < model.vertexs.size(); j++)
	//	{
	//		Vertex v = model.vertexs[j];
	//		
	//		XMMATRIX vp = gRender->getCamera()->ViewProj();
	//		XMMATRIX s = XMMatrixScaling(0.1f, 0.1f, 0.1f);
	//		vp = s*vp;
	//		XMFLOAT4 newPos = XMFLOAT4(v.Pos.x, v.Pos.y, v.Pos.z, 1.0f);
	//		XMVECTOR vector = XMVector4Transform(XMLoadFloat4(&newPos), vp);
	//		XMStoreFloat4(&newPos, vector);
	//		Vertex newVertex;
	//		newVertex.Pos = newPos;
	//		newVertex.UV = v.UV;
	//		newVertexs.push_back(newVertex);
	//	}

	//	obj->getGeometry()->updateVertexData(newVertexs);
	//}
}


void Mesh::skin()
{
	map<string, XMFLOAT4X4> matrixMap = curAnimation->GetPosMatrix();
	skinedMeshAry = subMeshAry;

	for (int indexSubMesh = 0; indexSubMesh < subMeshAry.size(); indexSubMesh++)
	{
		ModelData &modelData = subMeshAry[indexSubMesh];
		int curVertexIndex = -1;
		XMFLOAT3 curPos;
		for (int indexBoneAssign = 0; indexBoneAssign < modelData.boneVertexAssigns.size(); indexBoneAssign++)
		{
			BoneVertexAssignment &ass = modelData.boneVertexAssigns[indexBoneAssign];
			
			if (ass.vertexIndex != curVertexIndex)
			{
				if (curVertexIndex != -1)
				{
					skinedMeshAry[indexSubMesh].vertexs[curVertexIndex].Pos = curPos;
				}
				
				curVertexIndex = ass.vertexIndex;
				curPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
			}
			
			
			Vertex &v = subMeshAry[indexSubMesh].vertexs[ass.vertexIndex];
			Skeleton::Bone *bone = curAnimation->GetSkeleton()->GetBone(ass.boneIndex);

			XMFLOAT4 posTranslate;
			XMFLOAT4 vPos = XMFLOAT4(v.Pos.x, v.Pos.y, v.Pos.z, 1.0f);
			XMVECTOR vector = XMVector4Transform(XMLoadFloat4(&vPos), XMLoadFloat4x4(&bone->poseMatrix));
			XMStoreFloat4(&posTranslate, vector);
			curPos.x += posTranslate.x*ass.weight;
			curPos.y += posTranslate.y*ass.weight;
			curPos.z += posTranslate.z*ass.weight;

			//XMMATRIX m = XMMatrixRotationY(3.14f);
			//XMFLOAT4X4 mdata;
			//XMStoreFloat4x4(&mdata, m);
			//int ffjif = 0;
		}

		//for (map<int, XMFLOAT3>::iterator it = posMap.begin(); it != posMap.end(); it++)
		//{
		//	XMFLOAT3 pos = it->second;
		//	skinedMeshAry[indexSubMesh].vertexs[it->first].Pos = pos;
		//}
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

	//if (!IsPlayAnimation() || !curAnimation->HasDataChanged())
	//	modelAry;

	for (int i = 0; i < curAnimation->GetSkeleton()->GetBones().size(); i++)
	{
		Skeleton::Bone* b = curAnimation->GetSkeleton()->GetBone(i);
		XMFLOAT4 v = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		MyVertex::Vertex vertex;
		XMVECTOR vector = XMVector4Transform(XMLoadFloat4(&v), XMLoadFloat4x4(&b->globalMatrix));
		XMFLOAT4 fdata;
		XMStoreFloat4(&fdata, vector);
		//vertex.Pos = XMFLOAT3(fdata.x, fdata.y, fdata.z);
		data.vertexs.push_back(vertex);

		//if (b->name == "Waist")
		//	vertex.Color = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

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
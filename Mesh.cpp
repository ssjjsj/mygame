#include "Mesh.h"
using namespace MyVertex;
#include <map>
#include "MathHelp.h"
#include "global.h"
#include "Render.h"
#include "gpuResManger.h"
#include <algorithm>
using namespace std;

Mesh::Mesh()
{

}

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

	for (int i = 0; i < renderAbleList.size(); i++)
	{
		delete renderAbleList[i];
	}
}

void Mesh::loadFile(string filename)
{
	string format = filename.substr(filename.size() - 3, 3);
	if (format == "xml")
		subMeshAry = OrgeMeshPaser::parseMesh(ModelPath+filename);
	else if (format == "obj")
		subMeshAry = OrgeMeshPaser::parseObjMesh(ModelPath + filename);


}

void Mesh::setMaterial(string name) 
{ 
	Ptr<MaterialRes> res = ResManager::Instance().createMatRes(name);

	for (int i = 0; i < subMeshAry.size(); i++)
	{

		MyVertex::ModelData &model = subMeshAry[i];

		Geometry *g = new Geometry();
		g->setIndexData(model.indexs);
		g->setVertexData(model.vertexs);


		MaterialRes::MaterialData data = res->getMaterialData(model.materialName);
		Material *m = new Material(data);
		RenderAble *obj = new RenderAble(g, m);
		renderAbleList.push_back(obj);
	}
}

void Mesh::attachAnimation(string fileName)
{
	fileName = ModelPath + fileName + ".skeleton.xml";
	TiXmlDocument doc = TiXmlDocument(fileName.c_str());
	doc.LoadFile();

	TiXmlElement *skeletonRoot = doc.RootElement();
	TiXmlNode* animationRootNode = skeleton.loadFile(skeletonRoot->FirstChild());

	TiXmlNode *curAnimationNode;
	for (curAnimationNode = animationRootNode->FirstChild(); curAnimationNode != NULL; curAnimationNode = curAnimationNode->NextSibling())
	{
		TiXmlElement *animationElement = (TiXmlElement*)curAnimationNode;
		Animation *a = new Animation();
		a->SetSkeleton(&skeleton);
		a->loadAnimation(animationElement);
		animations.push_back(a);
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
			break;
		}
	}

	if (std::find(curAnimations.cbegin(), curAnimations.cend(), animation) == curAnimations.end())
		curAnimations.push_back(animation);
}


void Mesh::stopAnimation(string animationName)
{
	curAnimations.erase(std::remove_if(curAnimations.begin(), curAnimations.end(), [animationName](Animation* it) {return it->GetName() == animationName; }), curAnimations.end());
}



void Mesh::Update(float deltaTime)
{
	if (curAnimations.size() != 0)
	{
		for (int i = 0; i < curAnimations.size(); i++)
		{
			curAnimations[i]->update(deltaTime);
		}

		skin();
	}

	GameObject::Update(deltaTime);
}


void Mesh::skin()
{
	//skinedMeshAry = subMeshAry;
	//for (int indexAnimation = 0; indexAnimation < curAnimations.size(); indexAnimation++)
	//{
	//	Animation *curAnimation = curAnimations[indexAnimation];

	//	map<string, XMFLOAT4X4> matrixMap = curAnimation->GetPosMatrix();

	//	for (int indexSubMesh = 0; indexSubMesh < subMeshAry.size(); indexSubMesh++)
	//	{
	//		ModelData &modelData = subMeshAry[indexSubMesh];
	//		int curVertexIndex = -1;
	//		XMFLOAT3 curPos;
	//		for (int indexBoneAssign = 0; indexBoneAssign < modelData.boneVertexAssigns.size(); indexBoneAssign++)
	//		{
	//			BoneVertexAssignment &ass = modelData.boneVertexAssigns[indexBoneAssign];

	//			if (ass.vertexIndex != curVertexIndex)
	//			{
	//				if (curVertexIndex != -1)
	//				{
	//					skinedMeshAry[indexSubMesh].vertexs[curVertexIndex].Pos = curPos;
	//				}

	//				curVertexIndex = ass.vertexIndex;
	//				curPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//			}


	//			Vertex &v = subMeshAry[indexSubMesh].vertexs[ass.vertexIndex];
	//			Skeleton::Bone *bone = curAnimation->GetSkeleton()->GetBone(ass.boneIndex);

	//			XMFLOAT4 posTranslate;
	//			XMFLOAT4 vPos = XMFLOAT4(v.Pos.x, v.Pos.y, v.Pos.z, 1.0f);
	//			XMVECTOR vector = XMVector4Transform(XMLoadFloat4(&vPos), XMLoadFloat4x4(&bone->poseMatrix));
	//			XMStoreFloat4(&posTranslate, vector);
	//			curPos.x += posTranslate.x*ass.weight;
	//			curPos.y += posTranslate.y*ass.weight;
	//			curPos.z += posTranslate.z*ass.weight;
	//		}
		//}
	//}


	std::vector<XMFLOAT4X4> matrixs;
	bool firstAnimation = true;
	for (int indexAnimation = 0; indexAnimation < curAnimations.size(); indexAnimation++)
	{
		Animation *curAnimation = curAnimations[indexAnimation];
		std::vector<Skeleton::Bone*> bones = curAnimation->GetSkeleton()->GetBones();
		for (int i = 0; i < bones.size(); i++)
		{
			XMFLOAT4X4 &m = bones[i]->poseMatrix;
			if (firstAnimation)
				matrixs.push_back(m);
			else
			{
				XMFLOAT4X4 &m1 = matrixs[i];
				XMMATRIX matrix = XMLoadFloat4x4(&m)*XMLoadFloat4x4(&m1);
				XMStoreFloat4x4(&matrixs[i], matrix);
			}
		}
		firstAnimation = false;
	}

	UpdateSkinMatrixsBufferCommand *c = (UpdateSkinMatrixsBufferCommand*)(gRender->getBufferCommand("skinMatrix"));
	c->updateMatrix(matrixs);
}


void DrawSubMesh()
{

}
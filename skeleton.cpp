#include "skeleton.h"
#include "MathHelp.h"


Skeleton::Skeleton()
{

}


TiXmlNode * Skeleton::loadFile(TiXmlNode *bonesRootNode)
{
	for (TiXmlNode *curBoneNode = bonesRootNode->FirstChild();
		curBoneNode != NULL; curBoneNode = curBoneNode->NextSibling())
	{
		Bone *b = new Bone;
		TiXmlElement *curBoneElement = (TiXmlElement*)curBoneNode;
		b->id = atoi(curBoneElement->Attribute("id"));
		b->name = curBoneElement->Attribute("name");
		
		TiXmlElement *posElement = curBoneElement->FirstChildElement();
		XMFLOAT3 pos;
		pos.x = atof(posElement->Attribute("x"));
		pos.y = atof(posElement->Attribute("y"));
		pos.z = atof(posElement->Attribute("z"));

		TiXmlElement *rotationElement = (TiXmlElement*)posElement->NextSibling();
		float angle = atof(rotationElement->Attribute("angle"));
		TiXmlElement *axisElement = (TiXmlElement*)rotationElement->FirstChild();
		XMFLOAT3 axis;
		axis.x = atof(axisElement->Attribute("x"));
		axis.y = atof(axisElement->Attribute("y"));
		axis.z = atof(axisElement->Attribute("z"));

		b->localTranslate = pos;
		XMVECTOR v = XMQuaternionRotationAxis(XMLoadFloat3(&axis), angle);
		XMStoreFloat4(&b->loaclQuaternion, v);

		bones.push_back(b);
		boneNameMap[b->name] = b;
	}

	TiXmlNode *boneHierarchyRoot = bonesRootNode->NextSibling();
	for (TiXmlNode *curBone = boneHierarchyRoot->FirstChild();
		curBone != NULL; curBone = curBone->NextSibling())
	{
		TiXmlElement *curBoneElement = (TiXmlElement*)(curBone);
		string boneName = curBoneElement->Attribute("bone");
		string parentBoneName = curBoneElement->Attribute("parent");
		boneNameMap[boneName]->parent = boneNameMap[parentBoneName];
	}

	buildHierarchy();

	return boneHierarchyRoot->NextSibling();
}


void Skeleton::buildHierarchy()
{
	for (int i = 0; i < bones.size(); i++)
	{
		Bone *b = bones[i];
		if (b->parent == NULL)
			continue;
		string parentName = b->parent->name;

		if (boneNameMap.count(parentName) > 0)
		{
			boneNameMap[parentName]->children.push_back(b);
		}
	}

	buildInverseMatrix();
}


Skeleton::Bone* Skeleton::GetBone(string name)
{
	if (name == "Root")
	{
		if (boneNameMap.count(name) == 0)
			name = "root";
	}
	else if (name == "root")
	{
		if (boneNameMap.count(name) == 0)
			name = "Root";
	}


	if (boneNameMap.count(name) > 0)
	{
		return boneNameMap[name];
	}

	return NULL;
}



Skeleton::Bone* Skeleton::GetBone(int id)
{
	return bones[id];
}

void Skeleton::Bone::computePosMatrix()
{
	XMVECTOR locRotate = XMQuaternionMultiply(XMLoadFloat4(&inverseQuaternion), XMLoadFloat4(&globalQuaternion));
	XMMATRIX m = XMMatrixRotationQuaternion(locRotate);
	XMVECTOR translate = XMVector3Transform(XMLoadFloat3(&inverseTranslate), m);
	XMFLOAT3 translatef;

	XMStoreFloat3(&translatef, translate);
	translatef.x += globalTranslate.x;
	translatef.y += globalTranslate.y;
	translatef.z += globalTranslate.z;

	XMStoreFloat4x4(&posMatrix, m);
	posMatrix._41 = translatef.x;
	posMatrix._42 = translatef.y; 
	posMatrix._43 = translatef.z;

	for (int i = 0; i < children.size(); i++)
	{
		Bone *b = children[i];
		b->computePosMatrix();
	}
}


void Skeleton::Bone::reset()
{
	loaclQuaternion.x = 0.0f;
	loaclQuaternion.y = 0.0f;
	loaclQuaternion.z = 0.0f;
	loaclQuaternion.w = 1.0f;

	localTranslate.x = 0.0f;
	localTranslate.y = 0.0f;
	localTranslate.z = 0.0f;

	for (int i = 0; i < children.size(); i++)
	{
		Bone *b = children[i];
		b->reset();
	}
}

void Skeleton::Bone::updateTransform()
{
	if (parent != NULL)
	{
		XMVECTOR v = XMVectorMultiply(XMLoadFloat4(&parent->globalQuaternion), XMLoadFloat4(&loaclQuaternion));
		XMStoreFloat4(&globalQuaternion, v);

		XMMATRIX m = XMMatrixRotationQuaternion(XMLoadFloat4(&parent->globalQuaternion));
		XMVECTOR t = XMVector3Transform(XMLoadFloat3(&localTranslate), m);
		XMFLOAT3 translate;
		XMStoreFloat3(&translate, t);

		globalTranslate.x = parent->globalTranslate.x + translate.x;
		globalTranslate.y = parent->globalTranslate.y + translate.y;
		globalTranslate.z = parent->globalTranslate.z + translate.z;

		if (globalQuaternion.x == 0.0f && globalQuaternion.y == 0.0f && globalQuaternion.z == 0.0f)
		{
			globalQuaternion.w = 1.0f;
		}
	}
	else
	{
		globalQuaternion = loaclQuaternion;
		globalTranslate = localTranslate;
	}

	for (int i = 0; i < children.size(); i++)
	{
		Bone *b = children[i];
		b->updateTransform();
	}
}

void Skeleton::buildInverseMatrix()
{
	Bone *root = GetBone("root");
	root->updateTransform();

	for (int i = 0; i < bones.size(); i++)
	{
		Bone *b = bones[i];
		XMVECTOR q = XMLoadFloat4(&b->globalQuaternion);
		XMVECTOR inverseQ = XMQuaternionInverse(q);
		XMStoreFloat4(&b->inverseQuaternion, inverseQ);

		b->inverseTranslate.x = -b->globalTranslate.x;
		b->inverseTranslate.y = -b->globalTranslate.y;
		b->inverseTranslate.z = -b->globalTranslate.z;
	}
}
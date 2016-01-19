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

		b->initTranslate = pos;
		XMVECTOR v = XMQuaternionRotationAxis(XMLoadFloat3(&axis), angle);
		v=XMQuaternionNormalize(v);
		XMStoreFloat4(&b->initQuaternion, v);

		b->loaclQuaternion = b->initQuaternion;
		b->localTranslate = b->initTranslate;

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

void Skeleton::Bone::computeInverseMatrix()
{
	XMMATRIX m0 = XMMatrixRotationQuaternion(XMLoadFloat4(&initQuaternion));
	XMMATRIX m1 = XMMatrixTranslation(initTranslate.x, initTranslate.y, initTranslate.z);
	XMMATRIX m3 = XMMatrixInverse(&XMMatrixDeterminant(m0*m1), m0*m1);
	if (parent != NULL)
	{
		XMMATRIX iM = m3*XMLoadFloat4x4(&parent->inverseMatrix);
		XMStoreFloat4x4(&inverseMatrix, iM);
	}
	else
	{
		XMStoreFloat4x4(&inverseMatrix, m3);
	}


	if (name == "Waist")
		int i = 0;

	for (int i = 0; i < children.size(); i++)
	{
		Bone *b = children[i];
		b->computeInverseMatrix();
	}
}

void Skeleton::Bone::computePosMatrix()
{
	XMMATRIX m = XMLoadFloat4x4(&inverseMatrix)*XMLoadFloat4x4(&globalMatrix);
	XMStoreFloat4x4(&poseMatrix, m);

	if (isStatic)
		int i = 0;

	for (int i = 0; i < children.size(); i++)
	{
		Bone *b = children[i];
		b->computePosMatrix();
	}
}


void Skeleton::Bone::reset()
{
}

void Skeleton::Bone::updateTransform()
{
	if (parent != NULL)
	{
		XMMATRIX m1 = XMLoadFloat4x4(&parent->globalMatrix);
		XMMATRIX m2 = XMLoadFloat4x4(&localMatrix);

		XMStoreFloat4x4(&globalMatrix, m1*m2);
	}
	else
	{
		globalMatrix = localMatrix;
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
	root->computeInverseMatrix();

	for (int i = 0; i < bones.size(); i++)
	{
		Bone *b = bones[i];

		b->inverseMatrix._13 = -b->inverseMatrix._13;
		b->inverseMatrix._23 = -b->inverseMatrix._23;
		b->inverseMatrix._31 = -b->inverseMatrix._31;
		b->inverseMatrix._32 = -b->inverseMatrix._32;
		b->inverseMatrix._34 = -b->inverseMatrix._34;
		b->inverseMatrix._43 = -b->inverseMatrix._43;
	}
}
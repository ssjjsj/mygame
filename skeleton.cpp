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

void Skeleton::Bone::computePosMatrix()
{
	//XMVECTOR v;
	//XMMATRIX m = XMMatrixRotationQuaternion(XMLoadFloat4(&inverseQuaternion));
	//v = XMLoadFloat3(&globalTranslate);
	//XMVECTOR v1 = XMVector3Transform(v, m);
	//XMFLOAT3 translate;
	//XMStoreFloat3(&translate, v1);

	posTranslate.x = inverseTranslate.x + globalTranslate.x;
	posTranslate.y = inverseTranslate.y + globalTranslate.y;
	posTranslate.z = inverseTranslate.z + globalTranslate.z;

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
		if (parent != NULL && parent->name == "Root")
			int i = 3;
		XMVECTOR q = XMQuaternionMultiply(XMLoadFloat4(&parent->globalQuaternion), XMLoadFloat4(&loaclQuaternion));
		//globalQuaternion = MathUntil::QuaternionMupilyQuaternion(parent->globalQuaternion, loaclQuaternion);
		XMStoreFloat4(&globalQuaternion, q);

		//XMFLOAT3 translate = MathUntil::quaternionVector(parent->globalQuaternion, localTranslate);
		XMVECTOR v;
		XMMATRIX m = XMMatrixRotationQuaternion(XMLoadFloat4(&parent->globalQuaternion));
		v = XMLoadFloat3(&localTranslate);
		XMVECTOR v1 = XMVector3Transform(v, m);
		XMFLOAT3 translate;
		XMStoreFloat3(&translate, v1);

		globalTranslate.x = parent->globalTranslate.x + translate.x;
		globalTranslate.y = parent->globalTranslate.y + translate.y;
		globalTranslate.z = parent->globalTranslate.z + translate.z;
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
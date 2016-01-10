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
		v=XMQuaternionNormalize(v);
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
	XMFLOAT4 q = MathUntil::QuaternionMupilyQuaternion(globalQuaternion, inverseQuaternion);
	//XMFLOAT3 translatef = MathUntil::quaternionVector(q, inverseTranslate);

	XMVECTOR v;
	XMMATRIX m = XMMatrixRotationQuaternion(XMLoadFloat4(&q));
	v = XMLoadFloat3(&inverseTranslate);
	XMVECTOR v1 = XMVector3Transform(v, m);
	XMFLOAT3 translatef;
	XMStoreFloat3(&translatef, v1);

	translatef.x += globalTranslate.x;
	translatef.y += globalTranslate.y;
	translatef.z += globalTranslate.z;

	XMFLOAT3X3 rot3x3 = MathUntil::QuaternionToRotationMatrix(q);

	posMatrix._11 = rot3x3._11;
	posMatrix._12 = rot3x3._12;
	posMatrix._13 = rot3x3._13;
	posMatrix._14 = translatef.x;
	
	posMatrix._21 = rot3x3._21;
	posMatrix._22 = rot3x3._22;
	posMatrix._23 = rot3x3._23;
	posMatrix._24 = translatef.y;
	
	posMatrix._31 = rot3x3._31;
	posMatrix._32 = rot3x3._32;
	posMatrix._33 = rot3x3._33;
	posMatrix._34 = translatef.z;

	posMatrix._41 = 0.0f;
	posMatrix._42 = 0.0f;
	posMatrix._43 = 0.0f;
	posMatrix._44 = 0.0f;

	for (int i = 0; i < children.size(); i++)
	{
		Bone *b = children[i];
		b->computePosMatrix();
	}
}


void Skeleton::Bone::reset()
{
	//loaclQuaternion.x = 0.0f;
	//loaclQuaternion.y = 0.0f;
	//loaclQuaternion.z = 0.0f;
	//loaclQuaternion.w = 1.0f;

	//////XMVECTOR q = XMQuaternionRotationAxis(XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f)), 3.14f/2);
	//////XMStoreFloat4(&loaclQuaternion, q);

	////XMMATRIX m = XMMatrixRotationY(3.14f/2);
	////XMVECTOR q = XMQuaternionRotationMatrix(m);
	////XMStoreFloat4(&loaclQuaternion, q);

	////XMVECTOR v;
	////v = XMLoadFloat3(&XMFLOAT3(1.0f, 0.0f, 0.0f));
	////XMVECTOR v1 = XMVector3Transform(v, m);
	////XMFLOAT3 v2;
	////XMStoreFloat3(&v2, v1);

	//localTranslate.x = 0.0f;
	//localTranslate.y = 0.0f;
	//localTranslate.z = 0.0f;

	//for (int i = 0; i < children.size(); i++)
	//{
	//	Bone *b = children[i];
	//	b->reset();
	//}
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
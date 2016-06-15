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

		XMMATRIX m0 = XMMatrixRotationQuaternion(XMLoadFloat4(&b->initQuaternion));
		XMMATRIX m1 = XMMatrixTranslation(b->initTranslate.x, b->initTranslate.y, b->initTranslate.z);
		XMStoreFloat4x4(&b->localMatrix, m0*m1);

		b->localMatrix._13 *= -1.0f;
		b->localMatrix._23 *= -1.0f;
		b->localMatrix._33 *= -1.0f;
		b->localMatrix._43 *= -1.0f;

		b->localMatrix._31 *= -1.0f;
		b->localMatrix._32 *= -1.0f;
		b->localMatrix._33 *= -1.0f;
		b->localMatrix._34 *= -1.0f;

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
		XMMATRIX iM = XMLoadFloat4x4(&parent->inverseMatrix)*m3;
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

	for (int i = 0; i < children.size(); i++)
	{
		Bone *b = children[i];
		b->computePosMatrix();
	}
}


void Skeleton::Bone::reset()
{
}

void Skeleton::Bone::updateTransform(FILE *fp)
{
	globalMatrix = localMatrix;

	Bone *p = parent;

	XMMATRIX globalM = XMLoadFloat4x4(&globalMatrix);

	while (p != NULL)
	{
		//globalMatrix = p->localMatrix*globalMatrix;
		//if (name == "Stomach")
		//{
		//	fprintf(fp, "curGlobaM\n");
		//	XMFLOAT4X4 fff;
		//	XMStoreFloat4x4(&fff, globalM);
		//	MathUntil::printfMatrix(fff, fp);
		//}


		globalM = globalM*XMLoadFloat4x4(&p->localMatrix);
		//if (name == "Stomach")
		//{
		//	fprintf(fp, "%s\n", p->name.c_str());
		//	MathUntil::printfMatrix(p->localMatrix, fp);

		//	fprintf(fp, "result\n");
		//	XMFLOAT4X4 fff;
		//	XMStoreFloat4x4(&fff, globalM);
		//	MathUntil::printfMatrix(fff, fp);
		//}
		p = p->parent;
	}



	XMStoreFloat4x4(&globalMatrix, globalM);

	for (int i = 0; i < children.size(); i++)
	{
		Bone *b = children[i];
		b->updateTransform(fp);
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
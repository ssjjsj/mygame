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
		
		XMFLOAT4X4 temp = MathUntil::GetTransformMatrix(pos, axis, angle);
		XMMATRIX m = XMLoadFloat4x4(&temp);
		XMMATRIX tempM = XMMatrixInverse(&XMMatrixDeterminant(m), m);
		XMStoreFloat4x4(&b->inverseM, tempM);

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

	buildInverseMatrix(GetBone("Root"));
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

void Skeleton::buildInverseMatrix(Bone *b)
{
	if (!b->IsRootBone())
	{
		XMMATRIX curM = XMLoadFloat4x4(&b->inverseM);
		XMMATRIX parentM = XMLoadFloat4x4(&b->parent->inverseM);

		XMMATRIX m = curM*parentM;

		XMStoreFloat4x4(&b->inverseM, m);

		//XMMATRIX curposM = XMLoadFloat4x4(&b->inverseM);
		//XMMATRIX parentposM = XMLoadFloat4x4(&matrixMap[b->parent->name]);
		//XMMATRIX posM = parentposM*XMMatrixInverse(&XMMatrixDeterminant(curposM), curposM);
		//XMStoreFloat4x4(&matrixMap[b->name], posM);
	}
	else
	{
		//XMMATRIX curM = XMLoadFloat4x4(&b->inverseM);
		//XMMATRIX posM = XMMatrixInverse(&XMMatrixDeterminant(curM), curM);
		//XMStoreFloat4x4(&matrixMap[b->name], posM);
	}

	for (int i = 0; i < b->children.size(); i++)
	{
		buildInverseMatrix(b->children[i]);
	}
}
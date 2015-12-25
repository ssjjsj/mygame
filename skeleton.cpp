#include "skeleton.h"


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
		b->pos.x = atof(posElement->Attribute("x"));
		b->pos.y = atof(posElement->Attribute("y"));
		b->pos.z = atof(posElement->Attribute("z"));

		TiXmlElement *rotationElement = (TiXmlElement*)posElement->NextSibling();
		b->angle = atof(rotationElement->Attribute("angle"));
		TiXmlElement *axisElement = (TiXmlElement*)rotationElement->FirstChild();
		b->axis.x = atof(axisElement->Attribute("x"));
		b->axis.y = atof(axisElement->Attribute("y"));
		b->axis.z = atof(axisElement->Attribute("z"));

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
	buildOriginInverseMatrix();

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
}


Skeleton::Bone* Skeleton::GetBone(string name)
{
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


void Skeleton::buildOriginInverseMatrix()
{
	for (int i = 0; i < bones.size(); i++)
	{
		Bone *b = bones[i];
		XMMATRIX m = XMMatrixRotationAxis(XMLoadFloat3(&b->axis), b->angle);
		boneOriginInverseMatrix[b->name] = m;
		//boneOriginInverseMatrix[b->name] = XMMatrixInverse(&XMMatrixDeterminant(m), m);
	}
}


XMMATRIX& Skeleton::GetOriginInverseMatrix(string boneName)
{
	return boneOriginInverseMatrix[boneName];
}
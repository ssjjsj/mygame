#include "OrgeMeshPaser.h"
#include <vector>

vector<ModelData> OrgeMeshPaser::parseMeshUseShardedVertex(string fileName)
{
	TiXmlDocument doc = TiXmlDocument(fileName.c_str());
	doc.LoadFile();

	vector<ModelData> modelDatas;

	TiXmlElement *meshRoot = doc.RootElement();

	TiXmlElement *sharedgeometryNode = meshRoot->FirstChildElement();
	TiXmlNode *vertexbufferNode = sharedgeometryNode->FirstChild();

	vector<Vertex> vertexs;
	for (TiXmlNode *curVertexNode = vertexbufferNode->FirstChild(); curVertexNode != NULL; curVertexNode = curVertexNode->NextSibling())
	{
		Vertex v;
		v.Color = XMFLOAT4(1.0f, 10.f, 0.0f, 0.0f);
		TiXmlElement *curPosElememt = (TiXmlElement*)curVertexNode->FirstChildElement();

		float x = atof(curPosElememt->Attribute("x"));
		float y = atof(curPosElememt->Attribute("y"));
		float z = atof(curPosElememt->Attribute("z"));

		TiXmlElement *curNormalElement = (TiXmlElement*)curPosElememt->NextSibling();
		float nx = atof(curNormalElement->Attribute("x"));
		float ny = atof(curNormalElement->Attribute("y"));
		float nz = atof(curNormalElement->Attribute("z"));

		TiXmlElement *uvElement = (TiXmlElement*)curNormalElement->NextSibling();
		float fu = atof(uvElement->Attribute("u"));
		float fv = atof(uvElement->Attribute("v"));

		v.Pos.x = x;
		v.Pos.y = y;
		v.Pos.z = z;
		v.Nor.x = nx;
		v.Nor.y = ny;
		v.Nor.z = nz;
		v.UV.x = fu;
		v.UV.y = fv;
		vertexs.push_back(v);
	}

	TiXmlNode * subMeshRootNode = sharedgeometryNode->NextSibling();
	

	for (TiXmlNode *curSubMeshNode = subMeshRootNode->FirstChild();
		curSubMeshNode != NULL; curSubMeshNode = curSubMeshNode->NextSibling())
	{
		ModelData model;
		model.vertexs = vertexs;
		TiXmlElement *curSubMeshElement = (TiXmlElement*)(curSubMeshNode);
		TiXmlElement *rootFaceNode = curSubMeshElement->FirstChildElement();
		int faceCount = atoi(rootFaceNode->Attribute("count"));
		for (TiXmlNode *curFaceNode = rootFaceNode->FirstChild(); curFaceNode != NULL; curFaceNode = curFaceNode->NextSibling())
		{
			TiXmlElement *curFaceElement = (TiXmlElement*)(curFaceNode);
			vector<int> indexs;
			indexs.push_back(atoi(curFaceElement->Attribute("v1")));
			indexs.push_back(atoi(curFaceElement->Attribute("v2")));
			indexs.push_back(atoi(curFaceElement->Attribute("v3")));
			model.indexs.push_back(indexs);
		}
		modelDatas.push_back(model);
	}

	return modelDatas;
}


vector<ModelData> OrgeMeshPaser::parseMesh(string filename)
{
	TiXmlDocument doc = TiXmlDocument(filename.c_str());
	doc.LoadFile();

	vector<ModelData> modelDatas;

	TiXmlElement *meshRoot = doc.RootElement();

	TiXmlNode *subMeshRootNode = meshRoot->FirstChild();


	for (TiXmlNode *curSubMeshNode = subMeshRootNode->FirstChild();
		curSubMeshNode != NULL; curSubMeshNode = curSubMeshNode->NextSibling())
	{
		ModelData model;
		TiXmlElement *curSubMeshElement = (TiXmlElement*)(curSubMeshNode);
		TiXmlElement *rootFaceNode = curSubMeshElement->FirstChildElement();
		int faceCount = atoi(rootFaceNode->Attribute("count"));
		for (TiXmlNode *curFaceNode = rootFaceNode->FirstChild(); curFaceNode != NULL; curFaceNode = curFaceNode->NextSibling())
		{
			TiXmlElement *curFaceElement = (TiXmlElement*)(curFaceNode);
			vector<int> indexs;
			indexs.push_back(atoi(curFaceElement->Attribute("v1")));
			indexs.push_back(atoi(curFaceElement->Attribute("v2")));
			indexs.push_back(atoi(curFaceElement->Attribute("v3")));
			model.indexs.push_back(indexs);
		}

		TiXmlNode *geometryRootNode = rootFaceNode->NextSibling();
		TiXmlElement *geometryRootElement = (TiXmlElement*)geometryRootNode;
		int vertexCount = atoi(geometryRootElement->Attribute("vertexcount"));
		TiXmlElement *vertexRootElement = geometryRootNode->FirstChildElement();
		string hasPos = vertexRootElement->Attribute("positions");
		bool hasNormal = vertexRootElement->Attribute("normals") == "true";

		for (TiXmlNode *curVertexNode = vertexRootElement->FirstChild(); curVertexNode != NULL; curVertexNode = curVertexNode->NextSibling())
		{
			Vertex v;
			v.Color = XMFLOAT4(1.0f, 10.f, 0.0f, 0.0f);
			TiXmlElement *curPosElememt = (TiXmlElement*)curVertexNode->FirstChildElement();

			float x = atof(curPosElememt->Attribute("x"));
			float y = atof(curPosElememt->Attribute("y"));
			float z = atof(curPosElememt->Attribute("z"));

			TiXmlElement *curNormalElement = (TiXmlElement*)curPosElememt->NextSibling();
			float nx = atof(curNormalElement->Attribute("x"));
			float ny = atof(curNormalElement->Attribute("y"));
			float nz = atof(curNormalElement->Attribute("z"));

			v.Pos.x = x;
			v.Pos.y = y;
			v.Pos.z = z;
			v.Nor.x = nx;
			v.Nor.y = ny;
			v.Nor.z = nz;
			model.vertexs.push_back(v);
		}

		TiXmlNode *uvRootNode = vertexRootElement->NextSibling();
		int i = 0;
		for (TiXmlNode* curUVNode = uvRootNode->FirstChild(); curUVNode != NULL; curUVNode = curUVNode->NextSibling())
		{
			TiXmlElement *uvElement = curUVNode->FirstChildElement();
			model.vertexs[i].UV.x = atof(uvElement->Attribute("u"));
			model.vertexs[i].UV.y = atof(uvElement->Attribute("v"));
			i++;
		}

		modelDatas.push_back(model);
	}

	return modelDatas;
}
#include "OrgeMeshPaser.h"
#include <vector>
#include <map>

namespace OrgeMeshPaser
{
	vector<MyVertex::ModelData> parseMeshUseShardedVertex(string fileName)
	{
		vector<MyVertex::ModelData> modelDatas;
		
		TiXmlDocument doc = TiXmlDocument(fileName.c_str());
		doc.LoadFile();

		TiXmlNode *rootNode = doc.FirstChild();

		TiXmlNode * subMeshRootNode = rootNode->FirstChild();


		for (TiXmlNode *curSubMeshNode = subMeshRootNode->FirstChild();
			curSubMeshNode != NULL; curSubMeshNode = curSubMeshNode->NextSibling())
		{
			MyVertex::ModelData model;
			TiXmlElement *curSubMeshElement = (TiXmlElement*)(curSubMeshNode);
			model.materialName = curSubMeshElement->Attribute("material");
			TiXmlElement *rootFaceNode = curSubMeshElement->FirstChildElement();
			int faceCount = atoi(rootFaceNode->Attribute("count"));
			for (TiXmlNode *curFaceNode = rootFaceNode->FirstChild(); curFaceNode != NULL; curFaceNode = curFaceNode->NextSibling())
			{
				TiXmlElement *curFaceElement = (TiXmlElement*)(curFaceNode);
				model.indexs.push_back(atoi(curFaceElement->Attribute("v1")));
				model.indexs.push_back(atoi(curFaceElement->Attribute("v2")));
				model.indexs.push_back(atoi(curFaceElement->Attribute("v3")));
			}

			TiXmlNode *sharedgeometryNode = rootFaceNode->NextSibling();
			TiXmlNode *vertexbufferNode = sharedgeometryNode->FirstChild();
			for (TiXmlNode *curVertexNode = vertexbufferNode->FirstChild(); curVertexNode != NULL; curVertexNode = curVertexNode->NextSibling())
			{
				MyVertex::Vertex v;
				TiXmlElement *curPosElememt = (TiXmlElement*)curVertexNode->FirstChildElement();

				float x = atof(curPosElememt->Attribute("x"));
				float y = atof(curPosElememt->Attribute("y"));
				float z = atof(curPosElememt->Attribute("z"));

				TiXmlElement *curNormalElement = (TiXmlElement*)curPosElememt->NextSibling();
				float nx = atof(curNormalElement->Attribute("x"));
				float ny = atof(curNormalElement->Attribute("y"));
				float nz = atof(curNormalElement->Attribute("z"));

				TiXmlElement *uvElement = (TiXmlElement*)curNormalElement->NextSibling();

				float uvX = atof(uvElement->Attribute("v"));
				float uvY = atof(uvElement->Attribute("u"));

				v.Pos.x = x;
				v.Pos.y = y;
				v.Pos.z = -z;
				v.Nor.x = nx;
				v.Nor.y = ny;
				v.Nor.z = nz;
				v.UV.x = uvX;
				v.UV.y = uvY;
				model.vertexs.push_back(v);
			}

			//TiXmlNode *boneVertexAssignRootNode = sharedgeometryNode->NextSibling();
			//for (TiXmlNode *curBoneVertexNode = boneVertexAssignRootNode->FirstChild();
			//	curBoneVertexNode != NULL; curBoneVertexNode = curBoneVertexNode->NextSibling())
			//{
			//	BoneVertexAssignment ass;
			//	TiXmlElement *curBoneVertexElement = (TiXmlElement*)(curBoneVertexNode);
			//	ass.vertexIndex = atoi(curBoneVertexElement->Attribute("vertexindex"));
			//	ass.boneIndex = atoi(curBoneVertexElement->Attribute("boneindex"));
			//	ass.weight = atof(curBoneVertexElement->Attribute("weight"));
			//	model.boneVertexAssigns.push_back(ass);
			//}
			

			modelDatas.push_back(model);
		}
		
		return modelDatas;
	}


	vector<MyVertex::ModelData> parseMesh(string filename)
	{
		TiXmlDocument doc = TiXmlDocument(filename.c_str());
		doc.LoadFile();

		vector<MyVertex::ModelData> modelDatas;

		TiXmlElement *meshRoot = doc.RootElement();

		TiXmlNode *subMeshRootNode = meshRoot->FirstChild();


		for (TiXmlNode *curSubMeshNode = subMeshRootNode->FirstChild();
			curSubMeshNode != NULL; curSubMeshNode = curSubMeshNode->NextSibling())
		{
			MyVertex::ModelData model;
			TiXmlElement *curSubMeshElement = (TiXmlElement*)(curSubMeshNode);
			model.materialName = curSubMeshElement->Attribute("material");
			TiXmlElement *rootFaceNode = curSubMeshElement->FirstChildElement();
			int faceCount = atoi(rootFaceNode->Attribute("count"));
			for (TiXmlNode *curFaceNode = rootFaceNode->FirstChild(); curFaceNode != NULL; curFaceNode = curFaceNode->NextSibling())
			{
				TiXmlElement *curFaceElement = (TiXmlElement*)(curFaceNode);
				model.indexs.push_back(atoi(curFaceElement->Attribute("v3")));
				model.indexs.push_back(atoi(curFaceElement->Attribute("v2")));
				model.indexs.push_back(atoi(curFaceElement->Attribute("v1")));
			}

			TiXmlNode *geometryRootNode = rootFaceNode->NextSibling();
			TiXmlElement *geometryRootElement = (TiXmlElement*)geometryRootNode;
			int vertexCount = atoi(geometryRootElement->Attribute("vertexcount"));
			TiXmlElement *vertexRootElement = geometryRootNode->FirstChildElement();
			string hasPos = vertexRootElement->Attribute("positions");
			bool hasNormal = vertexRootElement->Attribute("normals") == "true";

			for (TiXmlNode *curVertexNode = vertexRootElement->FirstChild(); curVertexNode != NULL; curVertexNode = curVertexNode->NextSibling())
			{
				MyVertex::Vertex v;
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
				v.Pos.z = -z;
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

			TiXmlNode *boneVertexAssignRootNode = geometryRootNode->NextSibling();
			for (TiXmlNode *curBoneVertexNode = boneVertexAssignRootNode->FirstChild();
				curBoneVertexNode != NULL; curBoneVertexNode = curBoneVertexNode->NextSibling())
			{
				BoneVertexAssignment ass;
				TiXmlElement *curBoneVertexElement = (TiXmlElement*)(curBoneVertexNode);
				ass.vertexIndex = atoi(curBoneVertexElement->Attribute("vertexindex"));
				ass.boneIndex = atoi(curBoneVertexElement->Attribute("boneindex"));
				ass.weight = atof(curBoneVertexElement->Attribute("weight"));
				model.boneVertexAssigns.push_back(ass);
			}


			map<int, vector<BoneVertexAssignment*>> indexWeightMap;
			for (int i = 0; i < model.boneVertexAssigns.size(); i++)
			{
				BoneVertexAssignment &ass = model.boneVertexAssigns[i];
				indexWeightMap[ass.vertexIndex].push_back(&ass);
			}

			for (map<int, vector<BoneVertexAssignment*>>::iterator it = indexWeightMap.begin(); it != indexWeightMap.end(); it++)
			{
				vector<BoneVertexAssignment*> assVec = it->second;
				float weight = 0.0f;
				for (int i = 0; i < assVec.size(); i++)
				{
					if (i == assVec.size() - 1)
					{
						if (weight + assVec[i]->weight > 1.0f)
							assVec[i]->weight = 1.0f - weight;
						else if (weight + assVec[i]->weight < 1.0f)
							assVec[i]->weight = 1.0f - weight;
					}
					else
					{
						weight += assVec[i]->weight;
					}
				}
			}

			modelDatas.push_back(model);
		}

		return modelDatas;
	}
}
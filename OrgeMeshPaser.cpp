#include "OrgeMeshPaser.h"
#include <vector>
#include <map>
#include <fstream>

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


	vector<MyVertex::ModelData> parseObjMesh(string fileName)
	{
		vector<XMFLOAT3> posList;
		vector<XMFLOAT2> uvList;
		vector<XMFLOAT3> normalList;

		map<string, int> cache;
		vector<MyVertex::ModelData> data;
		vector<MyVertex::Vertex> vertexList;
		vector<UINT> indexs;
		string materialName;

		ifstream f;
		f.open(fileName, ios::in);

		if (f.is_open())
		{
			int a = 3;
		}

		while (!f.eof())
		{
			char buf[100];
			string line;
			line.clear();
			f.getline(buf, 100);
			line = string(buf);
			if (line.size() > 0)
			{
				if (line.substr(0, 6) == "mtllib")
				{
					int a = 3;
					//string matLibName = line.substr(7);
					//matDic = parseMat("media/" + matLibName);
				}
				else if (line.substr(0, 2) == "vt")
				{
					float u, v;
					sscanf(line.c_str(), "vt %f %f", &u, &v);
					XMFLOAT2 uv;
					uv.x = u;
					uv.y = v;

					uvList.push_back(uv);
				}
				else if (line.substr(0, 2) == "vn")
				{
					float x, y, z;
					sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
					XMFLOAT3 nor;
					nor.x = x;
					nor.y = y;
					nor.z = z;

					normalList.push_back(nor);
				}
				else if (line.substr(0, 1) == "v")
				{
					float x, y, z;
					sscanf(line.c_str(), "v  %f %f %f", &x, &y, &z);
					XMFLOAT3 pos;
					pos.x = x;
					pos.y = y;
					pos.z = z;

					posList.push_back(pos);
				}
				else if (line.substr(0, 6) == "usemtl")
				{
					if (vertexList.size() > 0)
					{
						MyVertex::ModelData m;
						m.vertexs = vertexList;
						m.indexs = indexs;
						m.materialName = materialName;
						data.push_back(m);

						//if (data.size() == 2)
						//{
						//	data.clear();
						//	data.push_back(m);
						//	break;
						//}

						vertexList.clear();
						indexs.clear();
						cache.clear();
					}

					materialName = line.substr(7, line.size() - 7);
				}
				else if (line.substr(0, 1) == "f")
				{
					string tempAry[3];
					char buf0[100], buf1[100], buf2[100];
					sscanf(line.c_str(), "f %s %s %s", buf0, buf1, buf2);
					tempAry[0] = string(buf0);
					tempAry[1] = string(buf1);
					tempAry[2] = string(buf2);

					for (int i = 2; i >= 0; i--)
					{
						string s = tempAry[i];
						if (s.empty())
							continue;

						int count = 0;
						for (int i = 0; i < s.size(); i++)
						{
							if (s[i] == '/')
								count++;
						}
						if (count == 0)
						{
							int posIndex = atoi(s.c_str());
							string cacheString = "pos" + s;
							if (cache.count(cacheString) > 0)
							{
								int index = cache[cacheString];
								indexs.push_back(index);
							}
							else
							{
								MyVertex::Vertex newVertex;
								newVertex.Pos = posList[posIndex];
								vertexList.push_back(newVertex);
								indexs[i] = vertexList.size() - 1;
								cache[cacheString] = vertexList.size() - 1;
							}
						}
						else if (count == 1)
						{
							int posIndex;
							int uvIndex;
							sscanf(s.c_str(), "%i/%i", &posIndex, &uvIndex);
							string cacheString;
							sprintf((char*)cacheString.c_str(), "pos%iuv%i", posIndex, uvIndex);
							if (cache.count(cacheString) > 0)
							{
								int index = cache[cacheString];
								indexs.push_back(index);
							}
							else
							{
								MyVertex::Vertex newVertex;
								newVertex.Pos = posList[posIndex];
								newVertex.UV = uvList[uvIndex];
								vertexList.push_back(newVertex);
								indexs[i] = vertexList.size() - 1;
								cache[cacheString] = vertexList.size() - 1;
							}
						}
						else if (count == 2)
						{
							int posIndex;
							int uvIndex;
							int norIndex;
							int num = 0;

							sscanf(s.c_str(), "%i/%i/%i", &posIndex, &uvIndex, &norIndex);
							string cacheString;
							char buf[100];
							sprintf(buf, "pos%iuv%inor%i", posIndex, uvIndex, norIndex);
							cacheString = string(buf);
							map<string, int>::iterator it = cache.find(cacheString);
							if (it != cache.end())
							{
								int index = it->second;
								indexs.push_back(index);
							}
							else
							{
								MyVertex::Vertex newVertex;
								newVertex.Pos = posList[posIndex - 1];
								newVertex.UV = uvList[uvIndex - 1];
								newVertex.Nor = normalList[norIndex - 1];
								vertexList.push_back(newVertex);
								indexs.push_back(vertexList.size() - 1);
								cache[cacheString] = vertexList.size() - 1;
							}
						}
					}
				}
			}
		}

		MyVertex::ModelData m;
		m.vertexs = vertexList;
		m.indexs = indexs;
		m.materialName = materialName;
		data.push_back(m);
		f.close();

		return data;
	}
}
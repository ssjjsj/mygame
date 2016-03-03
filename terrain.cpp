#include "terrain.h"
#include "geometry.h"
#include "material.h"

void Terrain::loadData(string data)
{
	heightData.xSize = 100;
	heightData.zSize = 100;

	for (int indexZ = 0; indexZ < heightData.zSize; indexZ++)
	{
		for (int indexX = 0; indexX < heightData.xSize; indexX++)
		{
			heightData.data.push_back(0);
		}
	}

	width = 1;
}


void Terrain::setHeight(int x, int z, char height)
{
	int xSize = heightData.xSize;
	heightData.data[xSize*z + x] = height;
}

char Terrain::getHeight(int x, int z)
{
	int xSize = heightData.xSize;
	return heightData.data[xSize*z + x];
}


void Terrain::generateRenderAbles()
{
	MyVertex::Vertex vertex;
	vector<MyVertex::Vertex> vertexs;
	vector<UINT> indexs;

	for (int indexZ = 0; indexZ < heightData.zSize; indexZ++)
	{
		for (int indexX = 0; indexX < heightData.xSize; indexX++)
		{
			vertex.Pos.x = indexX;
			vertex.Pos.y = getHeight(indexX, indexZ);
			vertex.Pos.z = indexZ;

			vertexs.push_back(vertex);
		}

		for (int indexX = 0; indexX < heightData.xSize-1; indexX++)
		{
			for (int indexZ = 0; indexZ < heightData.zSize - 1; indexZ++)
			{
				indexs.push_back(indexX + indexZ*heightData.xSize);
				indexs.push_back(indexX+1 + indexZ*heightData.xSize);
				indexs.push_back(indexX + 1 + indexZ*heightData.xSize+1);
				
				indexs.push_back(indexX + 1 + indexZ*heightData.xSize + 1);
				indexs.push_back(indexX + indexZ*heightData.xSize + 1);
				indexs.push_back(indexX + indexZ*heightData.xSize);
			}
		}
	}
	Geometry *g = new Geometry();
	g->setIndexData(indexs);
	g->setVertexData(vertexs);

	MaterialRes res = MaterialRes("ogre.material.xml");
	MaterialRes::MaterialData data = res.getMaterialData("terrain");
	Material *m = new Material(data);

	RenderAble *obj = new RenderAble(g, m);
	renderAbleList.push_back(obj);
}

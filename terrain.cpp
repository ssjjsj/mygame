#include "terrain.h"
#include "geometry.h"
#include "material.h"
#include "global.h"
#include "Render.h"

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
	scale = 1.0f;
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

	fractal();

	for (int indexZ = 0; indexZ < heightData.zSize; indexZ++)
	{
		for (int indexX = 0; indexX < heightData.xSize; indexX++)
		{
			vertex.Pos.x = indexX*scale;
			vertex.Pos.y = getHeight(indexX, indexZ);
			vertex.Pos.z = indexZ*scale;

			vertexs.push_back(vertex);
		}

		for (int indexZ = 0; indexZ < heightData.zSize - 1; indexZ++)
		{
			for (int indexX = 0; indexX < heightData.xSize - 1; indexX++)
			{
				indexs.push_back(indexX + 1 + (indexZ+1)*heightData.xSize);
				indexs.push_back(indexX + 1 + indexZ*heightData.xSize);
				indexs.push_back(indexX + indexZ*heightData.xSize);
				
				indexs.push_back(indexX + indexZ*heightData.xSize);
				indexs.push_back(indexX + (indexZ + 1)*heightData.xSize);
				indexs.push_back(indexX + 1 + (indexZ + 1)*heightData.xSize);
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

void Terrain::fractal()
{
	float maxHeight = 5.0f;
	float minHeight = 0.0f;

	int iteratorTime = 10;

	for (int curIterator = 0; curIterator < iteratorTime; curIterator++)
	{
		float height = (maxHeight - minHeight) / iteratorTime*(iteratorTime - curIterator);

		int startX = rand() % 100;
		int startZ = rand() % 100;
		int endX = rand() % 100;
		int endZ = rand() % 100;

		XMFLOAT2 randLine = XMFLOAT2(endX - startX, endZ - startZ);
		
		for (int x = 0; x < heightData.xSize; x++)
		{
			for (int z = 0; z < heightData.zSize; z++)
			{
				XMFLOAT2 dir = XMFLOAT2(x - startX, z - startZ);
				if (randLine.x*dir.x + randLine.y*dir.y > 0)
				{
					heightData.data[x + heightData.xSize*z] += height;
				}
			}
		}


	}
}

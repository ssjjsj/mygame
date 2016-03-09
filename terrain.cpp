#include "terrain.h"
#include "geometry.h"
#include "material.h"
#include "global.h"
#include "Render.h"

Terrain::Terrain()
{
	initQuadTree();
}

Terrain::~Terrain()
{
	destoryQuadTree();
}

void Terrain::loadData(string data)
{
	heightData.xSize = 128;
	heightData.zSize = 128;

	for (int indexZ = 0; indexZ < heightData.zSize; indexZ++)
	{
		for (int indexX = 0; indexX < heightData.xSize; indexX++)
		{
			heightData.data.push_back(0);
		}
	}
	scale = 1.0f;

	initQuadTree();
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
	//MyVertex::Vertex vertex;
	//vector<MyVertex::Vertex> vertexs;
	//vector<UINT> indexs;

	//fractal();

	//for (int indexZ = 0; indexZ < heightData.zSize; indexZ++)
	//{
	//	for (int indexX = 0; indexX < heightData.xSize; indexX++)
	//	{
	//		vertex.Pos.x = indexX*scale;
	//		vertex.Pos.y = getHeight(indexX, indexZ);
	//		vertex.Pos.z = indexZ*scale;

	//		vertexs.push_back(vertex);
	//	}

	//	for (int indexZ = 0; indexZ < heightData.zSize - 1; indexZ++)
	//	{
	//		for (int indexX = 0; indexX < heightData.xSize - 1; indexX++)
	//		{
	//			indexs.push_back(indexX + 1 + (indexZ+1)*heightData.xSize);
	//			indexs.push_back(indexX + 1 + indexZ*heightData.xSize);
	//			indexs.push_back(indexX + indexZ*heightData.xSize);
	//			
	//			indexs.push_back(indexX + indexZ*heightData.xSize);
	//			indexs.push_back(indexX + (indexZ + 1)*heightData.xSize);
	//			indexs.push_back(indexX + 1 + (indexZ + 1)*heightData.xSize);
	//		}
	//	}
	//}

	modelData.vertexs.clear();
	modelData.indexs.clear();

	generateRenderAblesOnQuadTree();

	Geometry *g = new Geometry();
	g->setIndexData(modelData.indexs);
	g->setVertexData(modelData.vertexs);

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

void Terrain::initQuadTree()
{
	QuadNode *node = new QuadNode;
	node->isSubdivided = true;
	node->length = 128.0f;
	rootNode = node;
	node->centerPos = XMFLOAT2(31.0f, 31.0f);

	createQuadNode(64.0f, rootNode);
}

void Terrain::createQuadNode(float length, QuadNode *parentNode)
{
	if (length > 1.0f)
	{
		QuadNode *node = new QuadNode;
		if (length / 2 > 1.0f)
			node->isSubdivided = true;
		else
			node->isSubdivided = false;
		node->length = length / 2;
		node->centerPos = XMFLOAT2(parentNode->centerPos.x - length / 4, parentNode->centerPos.y - length / 4);
		parentNode->subNodes.push_back(node);
		createQuadNode(length / 2, node);

		node = new QuadNode;
		if (length / 2 > 1.0f)
			node->isSubdivided = true;
		else
			node->isSubdivided = false;
		node->length = length / 2;
		node->centerPos = XMFLOAT2(parentNode->centerPos.x + length / 4, parentNode->centerPos.y - length / 4);
		parentNode->subNodes.push_back(node);
		createQuadNode(length / 2, node);

		node = new QuadNode;
		if (length / 2 > 1.0f)
			node->isSubdivided = true;
		else
			node->isSubdivided = false;
		node->length = length / 2;
		node->centerPos = XMFLOAT2(parentNode->centerPos.x - length / 4, parentNode->centerPos.y + length / 4);
		parentNode->subNodes.push_back(node);
		createQuadNode(length / 2, node);

		node = new QuadNode;
		if (length / 2 > 1.0f)
			node->isSubdivided = true;
		else
			node->isSubdivided = false;
		node->length = length / 2;
		node->centerPos = XMFLOAT2(parentNode->centerPos.x - length / 4, parentNode->centerPos.y + length / 4);
		parentNode->subNodes.push_back(node);
		createQuadNode(length / 2, node);
	}
}


void Terrain::destoryQuadNode(QuadNode *node)
{
	for (int i = 0; i < node->subNodes.size(); i++)
	{
		destoryQuadNode(node->subNodes[i]); 
	}
	node->subNodes.clear();
	delete node;
}

void Terrain::destoryQuadTree()
{
	destoryQuadNode(rootNode);
}

void Terrain::reSetNode(QuadNode *node)
{
	node->isSubdivided = false;
	for (int i = 0; i < node->subNodes.size(); i++)
	{
		reSetNode(node->subNodes[i]);
	}
}

void Terrain::generateRenderAblesOnQuadTree()
{
	renderQuadNode(rootNode);
}

void Terrain::renderQuadNode(QuadNode *node)
{
	if (node->isSubdivided)
	{
		for (int i = 0; i < node->subNodes.size(); i++)
		{
			renderQuadNode(node->subNodes[i]);
		}
	}
	else
	{
		int vertexIndex = modelData.vertexs.size();

		MyVertex::Vertex v;
		float x = node->centerPos.x + node->length / 2;
		float z = node->centerPos.y + node->length / 2;
		float y = getHeight(x, z);
		v.Pos = XMFLOAT3(x, y, z);
		modelData.vertexs.push_back(v);


		x = node->centerPos.x + node->length / 2;
		z = node->centerPos.y - node->length / 2;
		y = getHeight(x, z);
		v.Pos = XMFLOAT3(x, y, z);

		x = node->centerPos.x - node->length / 2;
		z = node->centerPos.y - node->length / 2;
		y = getHeight(x, z);
		v.Pos = XMFLOAT3(x, y, z);

		x = node->centerPos.x - node->length / 2;
		z = node->centerPos.y + node->length / 2;
		y = getHeight(x, z);
		v.Pos = XMFLOAT3(x, y, z);

		modelData.indexs.push_back(vertexIndex);
		modelData.indexs.push_back(vertexIndex + 1);
		modelData.indexs.push_back(vertexIndex + 2);

		modelData.indexs.push_back(vertexIndex + 2);
		modelData.indexs.push_back(vertexIndex + 3);
		modelData.indexs.push_back(vertexIndex);
	}
}

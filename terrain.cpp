#include "terrain.h"
#include "geometry.h"
#include "material.h"
#include "global.h"
#include "Render.h"

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
	destoryQuadTree();
}

void Terrain::loadData(string data)
{
	heightData.xSize = 65;
	heightData.zSize = 65;

	for (int indexZ = 0; indexZ < heightData.zSize; indexZ++)
	{
		for (int indexX = 0; indexX < heightData.xSize; indexX++)
		{
			heightData.data.push_back(0);
		}
	}
	scale = 1.0f;

	fractal();
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
	renderAbleList.clear();
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
	node->length = (heightData.xSize-1)*scale;
	rootNode = node;
	node->centerPos.first = (node->length) / 2;
	node->centerPos.second = (node->length) / 2;

	if (IsSubdivided(node->centerPos, node->length))
	{
		createQuadNode(node->length / 2, node);
		node->isSubdivided = true;
	}
	else
	{
		node->isSubdivided = false;
	}
}

void Terrain::createQuadNode(float length, QuadNode *parentNode)
{
	QuadNode *node = new QuadNode;
	node->length = length;
	node->centerPos.first = parentNode->centerPos.first - length / 2;
	node->centerPos.second = parentNode->centerPos.second - length / 2;
	parentNode->subNodes.push_back(node);
	if (IsSubdivided(node->centerPos, node->length))
	{
		createQuadNode(node->length / 2, node);
		node->isSubdivided = true;
	}
	else
	{
		node->isSubdivided = false;
	}

	node = new QuadNode;
	node->length = length;
	node->centerPos.first = parentNode->centerPos.first + length / 2;
	node->centerPos.second = parentNode->centerPos.second - length / 2;
	parentNode->subNodes.push_back(node);
	if (IsSubdivided(node->centerPos, node->length))
	{
		createQuadNode(node->length / 2, node);
		node->isSubdivided = true;
	}
	else
	{
		node->isSubdivided = false;
	}

	node = new QuadNode;
	node->length = length;
	node->centerPos.first = parentNode->centerPos.first - length / 2;
	node->centerPos.second = parentNode->centerPos.second + length / 2;
	parentNode->subNodes.push_back(node);
	if (IsSubdivided(node->centerPos, node->length))
	{
		createQuadNode(node->length / 2, node);
		node->isSubdivided = true;
	}
	else
	{
		node->isSubdivided = false;
	}

	node = new QuadNode;
	node->length = length;
	node->centerPos.first = parentNode->centerPos.first + length / 2;
	node->centerPos.second = parentNode->centerPos.second + length / 2;
	parentNode->subNodes.push_back(node);
	if (IsSubdivided(node->centerPos, node->length))
	{
		createQuadNode(node->length / 2, node);
		node->isSubdivided = true;
	}
	else
	{
		node->isSubdivided = false;
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

void Terrain::updateQuadTree()
{
	reSetNode(rootNode);
	updateQuadNode(rootNode);
	generateRenderAbles();
}

void Terrain::updateQuadNode(QuadNode *node)
{
	if (IsSubdivided(node->centerPos, node->length))
	{
		node->isSubdivided = true;
		for (int i = 0; i < node->subNodes.size(); i++)
		{
			updateQuadNode(node->subNodes[i]);
		}
	}
}


void Terrain::generateRenderAblesOnQuadTree()
{
	modelData.vertexs.clear();
	modelData.indexs.clear();
	renderQuadNode(rootNode);
}

void Terrain::generateRenderAblesOnQuad(QuadNode *node)
{
	int vertexIndex = modelData.vertexs.size();
	MyVertex::Vertex v;
	float x = node->centerPos.first + node->length / 2;
	float z = node->centerPos.second + node->length / 2;
	float y = getHeight(x, z);
	v.Pos = XMFLOAT3(x, y, z);
	modelData.vertexs.push_back(v);


	x = node->centerPos.first + node->length / 2;
	z = node->centerPos.second - node->length / 2;
	y = getHeight(x, z);
	v.Pos = XMFLOAT3(x, y, z);
	modelData.vertexs.push_back(v);

	x = node->centerPos.first - node->length / 2;
	z = node->centerPos.second - node->length / 2;
	y = getHeight(x, z);
	v.Pos = XMFLOAT3(x, y, z);
	modelData.vertexs.push_back(v);

	x = node->centerPos.first - node->length / 2;
	z = node->centerPos.second + node->length / 2;
	y = getHeight(x, z);
	v.Pos = XMFLOAT3(x, y, z);
	modelData.vertexs.push_back(v);

	modelData.indexs.push_back(vertexIndex);
	modelData.indexs.push_back(vertexIndex + 1);
	modelData.indexs.push_back(vertexIndex + 2);

	modelData.indexs.push_back(vertexIndex + 2);
	modelData.indexs.push_back(vertexIndex + 3);
	modelData.indexs.push_back(vertexIndex);
}

void Terrain::renderQuadNode(QuadNode *node)
{
	if (node->isSubdivided && node->subNodes.size()>0)
	{
		for (int i = 0; i < node->subNodes.size(); i++)
		{
			renderQuadNode(node->subNodes[i]);
		}
	}
	else
	{
		generateRenderAblesOnQuad(node);
	}
}

bool Terrain::IsSubdivided(std::pair<int, int> center, float length)
{
	float constant = 10.0f;
	Camera *c = gRender->getCamera();
	XMFLOAT3 camPos = c->GetPosition();
	XMFLOAT3 nodePos = XMFLOAT3(center.first, getHeight(center.first, center.second), center.second);
	float distance = fabsf(camPos.x - nodePos.x) + fabsf(camPos.y - nodePos.y) + fabsf(camPos.z - nodePos.z);

	return distance / (length*constant) < 1.0f;
}



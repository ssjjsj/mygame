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
	heightData.xSize = 513;
	heightData.zSize = 513;

	nodeLayout.resize((heightData.xSize - 1)*(heightData.zSize - 1));

	for (int indexZ = 0; indexZ < heightData.zSize; indexZ++)
	{
		for (int indexX = 0; indexX < heightData.xSize; indexX++)
		{
			heightData.data.push_back(0);
		}
	}
	scale = 1.0f;

	fractal();
	procedualTexture();
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
	float maxHeight = 10.0f;
	float minHeight = 0.0f;

	int iteratorTime = 10;

	for (int curIterator = 0; curIterator < iteratorTime; curIterator++)
	{
		float height = maxHeight - (maxHeight - minHeight) * curIterator *1.0f / iteratorTime;

		int startX = rand() % heightData.xSize;
		int startZ = rand() % heightData.zSize;
		int endX = rand() % heightData.xSize;
		int endZ = rand() % heightData.zSize;

		XMFLOAT2 randLine = XMFLOAT2(endX - startX, endZ - startZ);
		float length = sqrtf(randLine.x*randLine.x + randLine.y*randLine.y);
		randLine.x /= length;
		randLine.y /= length;
		
		int addNum = 0;
		for (int z = 0; z < heightData.zSize; z++)
		{
			for (int x = 0; x < heightData.xSize; x++)
			{
				XMFLOAT2 dir = XMFLOAT2(x, z);
				float length = sqrtf(dir.x*dir.x + dir.y*dir.y);
				dir.x /= length;
				dir.y /= length;
				if (randLine.x*dir.x + randLine.y*dir.y > 0)
				{
					heightData.data[x + heightData.xSize*z] += height;
					addNum++;
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
	node->parentNode = parentNode;
	node->pos = QuadNode::Pos::DownLeft;
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
	node->parentNode = parentNode;
	node->pos = QuadNode::Pos::UpLeft;
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
	node->parentNode = parentNode;
	node->pos = QuadNode::Pos::DownRight;
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
	node->parentNode = parentNode;
	node->pos = QuadNode::Pos::UpRight;
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
	int x = node->centerPos.first + node->length / 2;
	int z = node->centerPos.second + node->length / 2;
	int y = getHeight(x, z);
	v.Pos = XMFLOAT3(x, y, z);
	v.Nor = heightData.precent[x + z*heightData.xSize];
	v.UV = XMFLOAT2(1.0f, 1.0f);
	modelData.vertexs.push_back(v);


	x = node->centerPos.first + node->length / 2;
	z = node->centerPos.second - node->length / 2;
	y = getHeight(x, z);
	v.Pos = XMFLOAT3(x, y, z);
	v.Nor = heightData.precent[x + z*heightData.xSize];
	v.UV = XMFLOAT2(1.0f, 0.0f);
	modelData.vertexs.push_back(v);

	x = node->centerPos.first - node->length / 2;
	z = node->centerPos.second - node->length / 2;
	y = getHeight(x, z);
	v.Pos = XMFLOAT3(x, y, z);
	v.Nor = heightData.precent[x + z*heightData.xSize];
	v.UV = XMFLOAT2(0.0f, 0.0f);
	modelData.vertexs.push_back(v);

	x = node->centerPos.first - node->length / 2;
	z = node->centerPos.second + node->length / 2;
	y = getHeight(x, z);
	v.Pos = XMFLOAT3(x, y, z);
	v.Nor = heightData.precent[x + z*heightData.xSize];
	v.UV = XMFLOAT2(0.0f, 1.0f);
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


void Terrain::procedualTexture()
{
	HeightRegion groundReigion;
	groundReigion.minHeight = 0;
	groundReigion.optimalHeight = 12;
	groundReigion.maxHeight = 24;

	HeightRegion grassReigion;
	grassReigion.minHeight = 12;
	grassReigion.optimalHeight = 24;
	grassReigion.maxHeight = 50;

	vector<HeightRegion> heightRegions;
	heightRegions.push_back(groundReigion);
	heightRegions.push_back(grassReigion);


	for (int z = 0; z < heightData.zSize; z++)
	{
		for (int x = 0; x < heightData.xSize; x++)
		{
			XMFLOAT3 data = XMFLOAT3(0.0f, 0.0f, 0.0f);
			for (int i = 0; i < heightRegions.size(); i++)
			{
				HeightRegion& reigion = heightRegions[i];
				int index = x + z*heightData.xSize;
				float height = heightData.data[index];
				float precent = 0.0f;

				if (height <= reigion.minHeight)
				{
					precent = 0.0f;
				}
				else if (height <= reigion.optimalHeight && height > reigion.minHeight)
				{
					precent = (float)(height - reigion.minHeight) / (float)(reigion.optimalHeight - reigion.minHeight);
				}
				else if (height>reigion.optimalHeight && height < reigion.maxHeight)
				{
					precent = 1.0f - (float)(height - reigion.optimalHeight) / (float)(reigion.maxHeight - reigion.optimalHeight);
				}
				else if (height >= reigion.maxHeight)
				{
					precent = 0.0f;
				}

				if (i == 0)
					data.x = precent;
				else if (i == 1)
					data.y = precent;
			}
			heightData.precent.push_back(data);
		}
	}
}


bool Terrain::IsCracked(QuadNode *node)
{
	return false;
}

void Terrain::generateNodeLayoutOnQuad(QuadNode *node, int level)
{
	if (node->isSubdivided == false)
	{
		for (int x = 0; x < node->length; x++)
		{
			for (int z = 0; z < node->length; z++)
			{
				switch (node->pos)
				{
				case QuadNode::Pos::DownLeft:
					break;
				case QuadNode::Pos::DownRight:
					break;
				case QuadNode::Pos::UpLeft:
					break;
				case QuadNode::Pos::UpRight:
					break;
				}
			}
		}
	}
	else
	{

	}
}



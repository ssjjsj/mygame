#include "terrain.h"
#include "geometry.h"
#include "material.h"
#include "global.h"
#include "Render.h"
#include "gpuResManger.h"
#include "sceneManager.h"

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
	destoryQuadTree();

	for (int i = 0; i < renderAbleList.size(); i++)
		delete renderAbleList[i];
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

	//fractal();
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

	//vector<float> depth;
	//Camera *c = SceneManager::Instance().getLightCamera();
	//XMMATRIX vp = c->ViewProj();
	//for (int i = 0; i < modelData.vertexs.size(); i++)
	//{
	//	XMVECTOR v = XMLoadFloat4(&XMFLOAT4(modelData.vertexs[i].Pos.x, modelData.vertexs[i].Pos.y, modelData.vertexs[i].Pos.z, 1.0f));
	//	XMVECTOR v1 = XMVector4Transform(v, vp);
	//	XMFLOAT4 data;
	//	XMStoreFloat4(&data, v1);
	//	depth.push_back(data.z / data.w);
	//}

	Geometry *g = new Geometry();
	g->setIndexData(modelData.indexs);
	g->setVertexData(modelData.vertexs);



	Ptr<MaterialRes> res = Singleton<ResManager>::Instance().createMatRes("terrain.material.xml");
	MaterialRes::MaterialData data = res->getMaterialData("terrain");
	Material *m = new Material(data);
	
	RenderAble *obj = new RenderAble(g, m);
	renderAbleList.push_back(obj);

	//data = res->getMaterialData("terrainwire");
	//m = new Material(data);
	//obj = new RenderAble(g, m);
	//renderAbleList.push_back(obj);
}

void Terrain::fractal()
{
	float maxHeight = 10.0f;
	float minHeight = 0.0f;

	int iteratorTime = 4;

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

		XMFLOAT2 normalRandline;
		XMVECTOR v = XMVector2Orthogonal(XMLoadFloat2(&randLine));
		XMStoreFloat2(&normalRandline, v);
		
		for (int z = 0; z < heightData.zSize; z++)
		{
			for (int x = 0; x < heightData.xSize; x++)
			{
				XMFLOAT2 dir = XMFLOAT2(x-startX, z-startZ);
				float length = sqrtf(dir.x*dir.x + dir.y*dir.y);
				dir.x /= length;
				dir.y /= length;
				if (normalRandline.x*dir.x + normalRandline.y*dir.y > 0)
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

	generateNodeLayoutOnQuad(rootNode, 0);
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
	node->centerPos.first = parentNode->centerPos.first - length / 2;
	node->centerPos.second = parentNode->centerPos.second + length / 2;
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
	generateNodeLayoutOnQuad(rootNode, 0);
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


	MyVertex::Vertex rightUp;
	int x = node->centerPos.first + node->length / 2;
	int z = node->centerPos.second + node->length / 2;
	int y = getHeight(x, z);
	rightUp.Pos = XMFLOAT3(x, y, z);
	rightUp.Nor = heightData.precent[x + z*heightData.xSize];
	rightUp.UV = XMFLOAT2(1.0f, 1.0f);


	MyVertex::Vertex rightDown;
	x = node->centerPos.first + node->length / 2;
	z = node->centerPos.second - node->length / 2;
	y = getHeight(x, z);
	rightDown.Pos = XMFLOAT3(x, y, z);
	rightDown.Nor = heightData.precent[x + z*heightData.xSize];
	rightDown.UV = XMFLOAT2(1.0f, 0.0f);

	MyVertex::Vertex leftDown;
	x = node->centerPos.first - node->length / 2;
	z = node->centerPos.second - node->length / 2;
	y = getHeight(x, z);
	leftDown.Pos = XMFLOAT3(x, y, z);
	leftDown.Nor = heightData.precent[x + z*heightData.xSize];
	leftDown.UV = XMFLOAT2(0.0f, 0.0f);

	MyVertex::Vertex leftUp;
	x = node->centerPos.first - node->length / 2;
	z = node->centerPos.second + node->length / 2;
	y = getHeight(x, z);
	leftUp.Pos = XMFLOAT3(x, y, z);
	leftUp.Nor = heightData.precent[x + z*heightData.xSize];
	leftUp.UV = XMFLOAT2(0.0f, 1.0f);

	MyVertex::Vertex left;
	left.Pos.x = (leftUp.Pos.x + leftDown.Pos.x) / 2;
	left.Pos.z = (leftUp.Pos.z + leftDown.Pos.z) / 2;
	left.Pos.y = getHeight(left.Pos.x, left.Pos.z);
	left.Nor.x = (leftUp.Nor.x + leftDown.Nor.x) / 2;
	left.Nor.y = (leftUp.Nor.y + leftDown.Nor.y) / 2;
	left.Nor.z = (leftUp.Nor.z + leftDown.Nor.z) / 2;
	left.UV.x = (leftUp.UV.x + leftDown.UV.x) / 2;
	left.UV.y = (leftUp.UV.y + leftDown.UV.y) / 2;

	MyVertex::Vertex right;
	right.Pos.x = (rightUp.Pos.x + rightDown.Pos.x) / 2;
	right.Pos.z = (rightUp.Pos.z + rightDown.Pos.z) / 2;
	right.Pos.y = getHeight(right.Pos.x, right.Pos.z);
	right.Nor.x = (rightUp.Nor.x + rightDown.Nor.x) / 2;
	right.Nor.y = (rightUp.Nor.y + rightDown.Nor.y) / 2;
	right.Nor.z = (rightUp.Nor.z + rightDown.Nor.z) / 2;
	right.UV.x = (rightUp.UV.x + rightDown.UV.x) / 2;
	right.UV.y = (rightUp.UV.y + rightDown.UV.y) / 2;


	MyVertex::Vertex up;
	up.Pos.x = (leftUp.Pos.x + rightUp.Pos.x) / 2;
	up.Pos.z = (leftUp.Pos.z + rightUp.Pos.z) / 2;
	up.Pos.y = getHeight(up.Pos.x, up.Pos.z);
	up.Nor.x = (leftUp.Nor.x + rightUp.Nor.x) / 2;
	up.Nor.y = (leftUp.Nor.y + rightUp.Nor.y) / 2;
	up.Nor.z = (leftUp.Nor.z + rightUp.Nor.z) / 2;
	up.UV.x = (leftUp.UV.x + rightUp.UV.x) / 2;
	up.UV.y = (leftUp.UV.y + rightUp.UV.y) / 2;

	MyVertex::Vertex down;
	down.Pos.x = (leftDown.Pos.x + rightDown.Pos.x) / 2;
	down.Pos.z = (leftDown.Pos.z + rightDown.Pos.z) / 2;
	down.Pos.y = getHeight(down.Pos.x, down.Pos.z);
	down.Nor.x = (leftDown.Nor.x + rightDown.Nor.x) / 2;
	down.Nor.y = (leftDown.Nor.y + rightDown.Nor.y) / 2;
	down.Nor.z = (leftDown.Nor.z + rightDown.Nor.z) / 2;
	down.UV.x = (leftDown.UV.x + rightDown.UV.x) / 2;
	down.UV.y = (leftDown.UV.y + rightDown.UV.y) / 2;

	
	
	set<QuadNode::Pos> bPos = IsCracked(node);
	if (bPos.size() > 1)
	{
		int i = 3;
	}
	bool includeLeft = bPos.count(QuadNode::Left) > 0;
	bool includeRight = bPos.count(QuadNode::Right) > 0;
	bool includeDown = bPos.count(QuadNode::Down) > 0;
	bool includeUp = bPos.count(QuadNode::Up) > 0;
	
	if (includeDown && includeRight)
	{
		modelData.vertexs.push_back(leftDown);
		modelData.vertexs.push_back(down);
		modelData.vertexs.push_back(rightDown);
		modelData.vertexs.push_back(right);
		modelData.vertexs.push_back(rightUp);

		modelData.indexs.push_back(vertexIndex + 4);
		modelData.indexs.push_back(vertexIndex + 3);
		modelData.indexs.push_back(vertexIndex + 0);

		modelData.indexs.push_back(vertexIndex + 3);
		modelData.indexs.push_back(vertexIndex + 2);
		modelData.indexs.push_back(vertexIndex + 1);

		modelData.indexs.push_back(vertexIndex + 3);
		modelData.indexs.push_back(vertexIndex + 1);
		modelData.indexs.push_back(vertexIndex + 0);
	}
	else if (includeDown)
	{
		modelData.vertexs.push_back(leftDown);
		modelData.vertexs.push_back(down);
		modelData.vertexs.push_back(rightDown);
		modelData.vertexs.push_back(rightUp);

		modelData.indexs.push_back(vertexIndex + 3);
		modelData.indexs.push_back(vertexIndex + 1);
		modelData.indexs.push_back(vertexIndex + 0);

		modelData.indexs.push_back(vertexIndex + 3);
		modelData.indexs.push_back(vertexIndex + 2);
		modelData.indexs.push_back(vertexIndex + 1);
	}
	else if (includeRight)
	{
		modelData.vertexs.push_back(leftDown);
		modelData.vertexs.push_back(rightDown);
		modelData.vertexs.push_back(right);
		modelData.vertexs.push_back(rightUp);

		modelData.indexs.push_back(vertexIndex + 2);
		modelData.indexs.push_back(vertexIndex + 1);
		modelData.indexs.push_back(vertexIndex + 0);

		modelData.indexs.push_back(vertexIndex + 3);
		modelData.indexs.push_back(vertexIndex + 2);
		modelData.indexs.push_back(vertexIndex + 0);
	}
	else
	{
		modelData.vertexs.push_back(leftDown);
		modelData.vertexs.push_back(rightDown);
		modelData.vertexs.push_back(rightUp);

		modelData.indexs.push_back(vertexIndex + 2);
		modelData.indexs.push_back(vertexIndex + 1);
		modelData.indexs.push_back(vertexIndex + 0);
	}
	
	vertexIndex = modelData.vertexs.size();
	if (includeLeft && includeUp)
	{
		modelData.vertexs.push_back(leftDown);
		modelData.vertexs.push_back(left);
		modelData.vertexs.push_back(leftUp);
		modelData.vertexs.push_back(up);
		modelData.vertexs.push_back(rightUp);

		modelData.indexs.push_back(vertexIndex + 0);
		modelData.indexs.push_back(vertexIndex + 1);
		modelData.indexs.push_back(vertexIndex + 4);

		modelData.indexs.push_back(vertexIndex + 1);
		modelData.indexs.push_back(vertexIndex + 2);
		modelData.indexs.push_back(vertexIndex + 3);

		modelData.indexs.push_back(vertexIndex + 1);
		modelData.indexs.push_back(vertexIndex + 3);
		modelData.indexs.push_back(vertexIndex + 4);
	}
	else if (includeLeft)
	{
		modelData.vertexs.push_back(leftDown);
		modelData.vertexs.push_back(left);
		modelData.vertexs.push_back(leftUp);
		modelData.vertexs.push_back(rightUp);

		modelData.indexs.push_back(vertexIndex + 0);
		modelData.indexs.push_back(vertexIndex + 1);
		modelData.indexs.push_back(vertexIndex + 3);

		modelData.indexs.push_back(vertexIndex + 1);
		modelData.indexs.push_back(vertexIndex + 2);
		modelData.indexs.push_back(vertexIndex + 3);
	}
	else if (includeUp)
	{
		modelData.vertexs.push_back(leftDown);
		modelData.vertexs.push_back(leftUp);
		modelData.vertexs.push_back(up);
		modelData.vertexs.push_back(rightUp);

		modelData.indexs.push_back(vertexIndex + 0);
		modelData.indexs.push_back(vertexIndex + 1);
		modelData.indexs.push_back(vertexIndex + 2);

		modelData.indexs.push_back(vertexIndex + 0);
		modelData.indexs.push_back(vertexIndex + 2);
		modelData.indexs.push_back(vertexIndex + 3);
	}
	else
	{
		modelData.vertexs.push_back(leftDown);
		modelData.vertexs.push_back(leftUp);
		modelData.vertexs.push_back(rightUp);

		modelData.indexs.push_back(vertexIndex + 0);
		modelData.indexs.push_back(vertexIndex + 1);
		modelData.indexs.push_back(vertexIndex + 2);
	}
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
	if (length <= 2)
		return false;

	float constant = 10.0f;
	Camera *c = SceneManager::Instance().getMainCamera();
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

				//if (i == 0)
				//	data.x = precent;
				//else if (i == 1)
				//	data.y = precent;

				if (i == 0)
					data.x = 0.0f;
				else if (i == 1)
					data.y = 1.0f;
			}
			heightData.precent.push_back(data);
		}
	}
}


set<Terrain::QuadNode::Pos> Terrain::IsCracked(QuadNode *node)
{
	set<QuadNode::Pos> bPos;
	int downLeftNodeIndex = node->centerPos.first - node->length / 2 + (heightData.xSize - 1)*(node->centerPos.second - node->length / 2);
	int downRightNodeIndex = downLeftNodeIndex + node->length - 1;
	int upLeftNodeIndex = downLeftNodeIndex + (node->length - 1)*(heightData.xSize-1);
	int upRightNodeIndex = downLeftNodeIndex + (node->length - 1)*(heightData.xSize - 1) + node->length-1;


	int x = node->centerPos.first - node->length / 2;
	int z = node->centerPos.second - node->length / 2;
	if (x > 0 && x < heightData.xSize - 1)
	{
		if (nodeLayout[downLeftNodeIndex - 1] > nodeLayout[downLeftNodeIndex])
			bPos.insert(QuadNode::Pos::Left);
	}

	if (z>0 && z < heightData.zSize - 1)
	{
		if (nodeLayout[downLeftNodeIndex - heightData.zSize + 1] > nodeLayout[downLeftNodeIndex])
			bPos.insert(QuadNode::Pos::Down);
	}

	x = node->centerPos.first + node->length / 2;
	z = node->centerPos.second - node->length / 2;
	if (x > 0 && x < heightData.xSize - 1)
	{
		if (nodeLayout[downRightNodeIndex + 1] > nodeLayout[downRightNodeIndex])
			bPos.insert(QuadNode::Pos::Right);
	}


	x = node->centerPos.first - node->length / 2;
	z = node->centerPos.second + node->length / 2;
	if (z>0 && z < heightData.zSize - 1)
	{
		if (nodeLayout[upLeftNodeIndex + heightData.zSize - 1] > nodeLayout[upLeftNodeIndex])
			bPos.insert(QuadNode::Pos::Up);
	}

	return bPos;
}

void Terrain::generateNodeLayoutOnQuad(QuadNode *node, int level)
{
	if (node->isSubdivided == false)
	{
		int startIndex = node->centerPos.first - node->length/2 + (heightData.xSize - 1)*(node->centerPos.second - node->length/2);
		for (int x = 0; x < node->length; x++)
		{
			for (int z = 0; z < node->length; z++)
			{
				nodeLayout[startIndex + x + (heightData.xSize - 1)*z] = level;
			}
		}
	}
	else
	{
		for (int i = 0; i < node->subNodes.size(); i++)
		{
			QuadNode *childNode = node->subNodes[i];
			generateNodeLayoutOnQuad(childNode, level + 1);
		}
	}
}



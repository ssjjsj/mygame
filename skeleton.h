#ifndef SKLETON_H
#define SKLETON_H
#include <string>
#include "tinyxml\tinystr.h"
#include "tinyxml\tinyxml.h"
#include "Common\MathHelper.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

class Skeleton
{
public:
	struct Bone
	{
		Bone *parent = NULL;
		string name;
		int id;
		XMFLOAT4X4 inverseM;
		vector<Bone*> children;
	};

public:
	Skeleton();
	TiXmlNode* loadFile(TiXmlNode *bonesRootNode);
	void buildHierarchy();
	void buildInverseMatrix(Bone *b);
	Bone* GetBone(string name);
	Bone* GetBone(int id);

private:
	vector<Bone*> bones;
	map<string, Bone*> boneNameMap;
};

#endif
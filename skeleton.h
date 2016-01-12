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
		XMFLOAT4 loaclQuaternion;
		XMFLOAT3 localTranslate;
		XMFLOAT4 initQuaternion;
		XMFLOAT3 initTranslate;
		XMFLOAT4 globalQuaternion;
		XMFLOAT3 globalTranslate;
		XMFLOAT4 inverseQuaternion;
		XMFLOAT3 inverseTranslate;
		vector<Bone*> children;

		XMFLOAT3 posTranslate;

		bool isStatic = false;

		bool IsRootBone()
		{
			return name == "root" || name == "Root";
		}

		void updateTransform();
		void computePosMatrix();
		void reset();
	};

public:
	Skeleton();
	TiXmlNode* loadFile(TiXmlNode *bonesRootNode);
	void buildHierarchy();
	void buildInverseMatrix();
	Bone* GetBone(string name);
	Bone* GetBone(int id);
	vector<Bone*> GetBones()
	{
		return bones;
	}
	map<string, XMFLOAT4X4> matrixMap;

private:
	vector<Bone*> bones;
	map<string, Bone*> boneNameMap;
};

#endif
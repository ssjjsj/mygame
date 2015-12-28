#ifndef ANIMATION_H
#define ANIMATION_H
#include "skeleton.h"
#include <string>
#include <vector>
using namespace::std;

class Animation
{
	struct KeyFrame
	{
		float startTime;
		XMFLOAT3 translate;
		XMFLOAT3 axis;
		float angle;
	};
	struct Track
	{
		string BoneName;
		vector<KeyFrame> keyFrames;
	};
public:
	Animation();
	Animation(string fileName);
	void loadFile(string fileName);
	void update(float time);
	void updateAllMatrix(map<string, XMFLOAT4X4>& matrixMap);
	void updateChildrenMatrix(Skeleton::Bone* bone, map<string, XMFLOAT4X4>& localMatrixMap);
	map<string, XMFLOAT4X4>& GetPosMatrix();
	Skeleton* GetSkeleton()
	{
		return &skeleton;
	}

	string GetName()
	{
		return animationName;
	}
private:
	void loadAnimations(TiXmlNode *rootNode);
	vector<KeyFrame*> findTwoKeyframes(float time, Track &t);
	XMFLOAT4X4 computePosMatrix(float time, KeyFrame *leftFrame, KeyFrame *rightFrame);
	XMFLOAT4X4 computePosMatrix(KeyFrame *frame);
private:
	Skeleton skeleton;
	string animationName;
	float timeLength;
	vector<Track> tracks;
	map<string, XMFLOAT4X4> posMatrix;
};
#endif
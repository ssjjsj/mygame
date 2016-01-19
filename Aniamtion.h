#ifndef ANIMATION_H
#define ANIMATION_H
#include "skeleton.h"
#include <string>
#include <vector>
#include "Vertex.h"
using namespace::std;

class Animation
{
	struct KeyFrame
	{
		float startTime;
		XMFLOAT4 quaternion;
		XMFLOAT3 translate;
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
	void updateAllMatrix();
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

	float GetAnimationLength()
	{
		return timeLength;
	}

	bool HasDataChanged()
	{
		return dataChanged;
	}
private:
	void loadAnimations(TiXmlNode *rootNode);
	vector<KeyFrame*> findTwoKeyframes(float time, Track &t);
	void computePosMatrix(float time, KeyFrame *leftFrame, KeyFrame *rightFrame, Skeleton::Bone *b);
	void computePosMatrix(KeyFrame *frame, Skeleton::Bone *b);
private:
	Skeleton skeleton;
	string animationName;
	float timeLength;
	vector<Track> tracks;
	map<string, XMFLOAT4X4> posMatrix;
	float curTime;

	KeyFrame *curLeftFrame;
	KeyFrame *curRightFrame;
	bool dataChanged;
};
#endif
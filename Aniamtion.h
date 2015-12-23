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
	void updateAllMatrix(map<string, XMMATRIX&>& matrixMap);
	void updateChildrenMatrix(Skeleton::Bone* bone, map<string, XMMATRIX&>& localMatrixMap);
	map<string, XMMATRIX>& GetPosMatrix();
private:
	void loadAnimations(TiXmlNode *rootNode);
	vector<KeyFrame*> findTwoKeyframes(float time, Track &t);
	XMMATRIX getPosMatrix(float time, KeyFrame *leftFrame, KeyFrame *rightFrame);
private:
	Skeleton skeleton;
	string animationName;
	float timeLength;
	vector<Track> tracks;
	map<string, XMMATRIX> posMatrix;
};
#endif
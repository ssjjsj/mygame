#include "Aniamtion.h"
#include "tinyxml\tinystr.h"
#include "tinyxml\tinyxml.h"
#include "skeleton.h"
#include "MathHelp.h"

Animation::Animation()
{

}


Animation::Animation(string fileName)
{
	loadFile(fileName);
}


map<string, XMMATRIX>& Animation::GetPosMatrix()
{
	return posMatrix;
}


void Animation::loadFile(string fileName)
{
	TiXmlDocument doc = TiXmlDocument(fileName.c_str());
	doc.LoadFile();

	TiXmlElement *skeletonRoot = doc.RootElement();
	TiXmlNode *bonesRootNode = skeletonRoot->FirstChild();

	TiXmlNode *animationRootNode = skeleton.loadFile(bonesRootNode);

	loadAnimations(animationRootNode);
}


void Animation::loadAnimations(TiXmlNode *rootNode)
{
	for (TiXmlNode *curAnimationNode = rootNode->FirstChild(); curAnimationNode != NULL; curAnimationNode = curAnimationNode->NextSibling())
	{
		TiXmlElement *curAnimationElement = (TiXmlElement*)(curAnimationNode);
		animationName = curAnimationElement->Attribute("name");
		timeLength = atof(curAnimationElement->Attribute("length"));

		TiXmlNode *trackRootNode = curAnimationElement->FirstChild();
		for (TiXmlNode *curTrackNode = trackRootNode->FirstChild();
			curTrackNode != NULL; curTrackNode->NextSibling())
		{
			Track t;
			TiXmlElement *curTrackElement = (TiXmlElement*)(curTrackNode);
			t.BoneName = curTrackElement->Attribute("bone");
			TiXmlNode *keyframesRootNode = curTrackElement->FirstChild();
			for (TiXmlNode *curKeyframeNode = keyframesRootNode->FirstChild();
				curKeyframeNode != NULL; curKeyframeNode = curKeyframeNode->NextSibling())
			{
				KeyFrame k;
				TiXmlElement *curKeyframeElement = (TiXmlElement*)(curKeyframeNode);

				TiXmlElement *translateElemnt = curKeyframeElement->FirstChildElement();
				k.translate.x = atof(translateElemnt->Attribute("x"));
				k.translate.y = atof(translateElemnt->Attribute("y"));
				k.translate.z = atof(translateElemnt->Attribute("z"));

				TiXmlElement *rotateElement = (TiXmlElement*)translateElemnt->NextSibling();
				k.angle = atof(rotateElement->Attribute("angle"));
				TiXmlElement *axisElement = rotateElement->FirstChildElement();
				k.axis.x = atof(axisElement->Attribute("x"));
				k.axis.y = atof(axisElement->Attribute("y"));
				k.axis.z = atof(axisElement->Attribute("z"));

				t.keyFrames.push_back(k);
			}
			tracks.push_back(t);
		}
	}
}


void Animation::update(float time)
{
	map<string, XMMATRIX> boneMatrixMap;
	for (int i = 0; i < tracks.size(); i++)
	{
		Track &t = tracks[i];
		vector<KeyFrame*> twoKeyFrame = findTwoKeyframes(time, t);
		KeyFrame *leftFrame = twoKeyFrame[0];
		KeyFrame *rightFrame = twoKeyFrame[1];

		if (leftFrame != NULL && rightFrame != NULL)
		{
			boneMatrixMap[t.BoneName] = getPosMatrix(time, leftFrame, rightFrame);
		}
	}
	updateAllMatrix(boneMatrixMap);
}


void Animation::updateAllMatrix(map<string, XMMATRIX>& matrixMap)
{
	posMatrix.clear();
	Skeleton::Bone *rootBone = skeleton.GetBone("Root");
	updateChildrenMatrix(rootBone, matrixMap);
}


void Animation::updateChildrenMatrix(Skeleton::Bone* bone, map<string, XMMATRIX>& localMatrixMap)
{
	XMMATRIX m;
	if (localMatrixMap.count(bone->name))
	{
		m = localMatrixMap[bone->name];
	}
	else
	{
		m = XMMatrixIdentity();
	}
	posMatrix[bone->name] = posMatrix[bone->parent->name] * m;

	for (int i = 0; i < bone->children.size(); i++)
	{
		updateChildrenMatrix(bone->children[i], localMatrixMap);
	}
}



XMMATRIX Animation::getPosMatrix(float time, KeyFrame *leftFrame, KeyFrame *rightFrame)
{
	XMMATRIX m = XMMatrixIdentity();

	XMFLOAT3 translate = MathUntil::lerpFloat3(leftFrame->translate, rightFrame->translate, leftFrame->startTime, time, rightFrame->startTime);
	XMVECTOR quaternion = XMVectorLerp(MathUntil::axisAngleToQuaternion(leftFrame->axis, leftFrame->angle),
		MathUntil::axisAngleToQuaternion(rightFrame->axis, rightFrame->angle), (time - leftFrame->startTime) / (rightFrame->startTime - leftFrame->startTime));

	XMMATRIX m1 = XMMatrixTranslation(translate.x, translate.y, translate.z);
	XMMATRIX m2 = XMMatrixRotationQuaternion(quaternion);

	return m*m1*m2;
}

vector<Animation::KeyFrame*> Animation::findTwoKeyframes(float time, Track &t)
{
	KeyFrame *leftFrame = NULL;
	KeyFrame *rightFrame = NULL;

	for (int i = 1; i < t.keyFrames.size(); i++)
	{
		KeyFrame& k = t.keyFrames[i];

		if (k.startTime <= time)
		{
			if (leftFrame != NULL)
			{
				if (k.startTime> leftFrame->startTime)
					leftFrame = &k;
			}
			else
			{
				leftFrame = &k;
			}
		}

		if (k.startTime >= time)
		{
			if (rightFrame != NULL)
			{
				if (k.startTime < rightFrame->startTime)
					rightFrame = &k;
			}
			else
			{
				rightFrame = &k;
			}
		}
	}

	vector<KeyFrame*> result;
	result.push_back(leftFrame);
	result.push_back(rightFrame);

	return result;
}

#include "Aniamtion.h"
#include "tinyxml\tinystr.h"
#include "tinyxml\tinyxml.h"
#include "skeleton.h"
#include "MathHelp.h"

Animation::Animation()
{

}


Animation::Animation(string animationName)
{
	string fileName = animationName + ".SKELETON.xml";
	loadFile(fileName);

	string skeletonFileName = "Cat.SKELETON.xml";
	TiXmlDocument doc = TiXmlDocument(skeletonFileName.c_str());
	doc.LoadFile();
	TiXmlNode *skeletonRoot = doc.RootElement()->FirstChild();
	skeleton.loadFile(skeletonRoot);
}


map<string, XMFLOAT4X4>& Animation::GetPosMatrix()
{
	return posMatrix;
}


void Animation::loadFile(string fileName)
{
	TiXmlDocument doc = TiXmlDocument(fileName.c_str());
	doc.LoadFile();

	TiXmlElement *skeletonRoot = doc.RootElement();
	TiXmlNode* animationRootNode = skeleton.loadFile(skeletonRoot->FirstChild());

	loadAnimations(animationRootNode);
}


void Animation::loadAnimations(TiXmlNode *rootNode)
{
	TiXmlNode *curAnimationNode = rootNode->FirstChild();
	TiXmlElement *curAnimationElement = (TiXmlElement*)(curAnimationNode);
	animationName = curAnimationElement->Attribute("name");
	timeLength = atof(curAnimationElement->Attribute("length"));

	TiXmlNode *trackRootNode = curAnimationElement->FirstChild();
	for (TiXmlNode *curTrackNode = trackRootNode->FirstChild();
		curTrackNode != NULL; curTrackNode = curTrackNode->NextSibling())
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
			k.startTime = atof(curKeyframeElement->Attribute("time"));

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


void Animation::update(float time)
{
	map<string, XMFLOAT4X4> boneMatrixMap;
	for (int i = 0; i < tracks.size(); i++)
	{
		Track &t = tracks[i];
		vector<KeyFrame*> twoKeyFrame = findTwoKeyframes(time, t);
		KeyFrame *leftFrame = twoKeyFrame[0];
		KeyFrame *rightFrame = twoKeyFrame[1];

		if (leftFrame != NULL && rightFrame != NULL)
		{
			if (leftFrame == rightFrame)
				boneMatrixMap[t.BoneName] = computePosMatrix(leftFrame);
			else
				boneMatrixMap[t.BoneName] = computePosMatrix(time, leftFrame, rightFrame);
		}
	}
	updateAllMatrix(boneMatrixMap);
}


void Animation::updateAllMatrix(map<string, XMFLOAT4X4>& matrixMap)
{
	posMatrix.clear();
	Skeleton::Bone *rootBone = skeleton.GetBone("root");
	updateChildrenMatrix(rootBone, matrixMap);
}


void Animation::updateChildrenMatrix(Skeleton::Bone* bone, map<string, XMFLOAT4X4>& localMatrixMap)
{
	XMMATRIX m;
	XMMATRIX localMatrix;
	if (localMatrixMap.count(bone->name))
	{
		m = XMLoadFloat4x4(&localMatrixMap[bone->name]);
	}
	else
	{
		m = XMMatrixIdentity();
	}
	if (bone->IsRootBone())
		XMStoreFloat4x4(&posMatrix[bone->name], m);
	else
	{
		XMMATRIX result = XMLoadFloat4x4(&posMatrix[bone->parent->name])*m;
		XMStoreFloat4x4(&posMatrix[bone->name], result);
	}

	for (int i = 0; i < bone->children.size(); i++)
	{
		updateChildrenMatrix(bone->children[i], localMatrixMap);
	}
}



XMFLOAT4X4 Animation::computePosMatrix(float time, KeyFrame *leftFrame, KeyFrame *rightFrame)
{
	XMFLOAT3 translate = MathUntil::lerpFloat3(leftFrame->translate, rightFrame->translate, leftFrame->startTime, time, rightFrame->startTime);
	XMVECTOR leftV = XMLoadFloat3(&leftFrame->axis);
	XMVECTOR leftQ = XMQuaternionRotationAxis(leftV, leftFrame->angle);

	XMVECTOR rightV = XMLoadFloat3(&rightFrame->axis);
	XMVECTOR rightQ = XMQuaternionRotationAxis(rightV, rightFrame->angle);

	XMVECTOR quaternion = XMVectorLerp(leftQ, rightQ, (time - leftFrame->startTime) / (rightFrame->startTime - leftFrame->startTime));

	XMMATRIX m1 = XMMatrixTranslation(translate.x, translate.y, translate.z);
	XMMATRIX m2 = XMMatrixRotationQuaternion(quaternion);

	XMFLOAT4X4 result;
	XMStoreFloat4x4(&result, m2*m1);
	return result;
}

XMFLOAT4X4 Animation::computePosMatrix(KeyFrame *frame)
{
	XMMATRIX m1 = XMMatrixTranslation(frame->translate.x, frame->translate.y, frame->translate.z);
	XMVECTOR v = XMLoadFloat3(&frame->axis);
	XMVECTOR q = XMQuaternionRotationAxis(v, frame->angle);
	XMMATRIX m2 = XMMatrixRotationQuaternion(q);
	XMFLOAT4X4 result;
	XMStoreFloat4x4(&result, m2*m1);
	return result;
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

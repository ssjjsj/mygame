#include "Aniamtion.h"
#include "tinyxml\tinystr.h"
#include "tinyxml\tinyxml.h"
#include "skeleton.h"
#include "MathHelp.h"
#include <stdio.h>
using namespace std;

Animation::Animation()
{

}


Animation::Animation(string animationName)
{
	string fileName = animationName + ".SKELETON.xml";
	loadFile(fileName);

	//string skeletonFileName = "Cat.SKELETON.xml";
	//TiXmlDocument doc = TiXmlDocument(skeletonFileName.c_str());
	//doc.LoadFile();
	//TiXmlNode *skeletonRoot = doc.RootElement()->FirstChild();
	//skeleton.loadFile(skeletonRoot);

	curTime = 0.0f;
	curLeftFrame = NULL;
	curRightFrame = NULL;
	dataChanged = false;
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
			float angle = atof(rotateElement->Attribute("angle"));
			TiXmlElement *axisElement = rotateElement->FirstChildElement();
			XMFLOAT3 axis;
			axis.x = atof(axisElement->Attribute("x"));
			axis.y = atof(axisElement->Attribute("y"));
			axis.z = atof(axisElement->Attribute("z"));

			XMVECTOR qv = XMQuaternionRotationAxis(XMLoadFloat3(&axis), angle);
			XMStoreFloat4(&k.quaternion, qv);

			Skeleton::Bone *b = skeleton.GetBone(t.BoneName);
			XMMATRIX m0 = XMMatrixRotationQuaternion(XMLoadFloat4(&b->initQuaternion));
			XMMATRIX m1 = XMMatrixTranslation(b->initTranslate.x, b->initTranslate.y, b->initTranslate.z);
			
			XMMATRIX m2 = XMMatrixRotationQuaternion(XMLoadFloat4(&k.quaternion));
			XMMATRIX m3 = XMMatrixTranslation(k.translate.x, k.translate.y, k.translate.z);

			XMMATRIX defaultPose = m0*m1;
			XMMATRIX poseM = m2*m3;

			XMMATRIX finalM = poseM*defaultPose;

			XMVECTOR quaternion = XMQuaternionRotationMatrix(finalM);
			
			XMFLOAT4X4 mData;
			XMStoreFloat4x4(&mData, finalM);
			k.translate.x = mData._41;
			k.translate.y = mData._42;
			k.translate.z = mData._43;

			XMStoreFloat4(&k.quaternion, quaternion);

			k.translate.z = -k.translate.z;
			k.quaternion.x = -k.quaternion.x;
			k.quaternion.y = -k.quaternion.y;

			t.keyFrames.push_back(k);
		}
		tracks.push_back(t);
	}
}


void Animation::update(float deltaTime)
{
	dataChanged = false;
 	curTime += deltaTime;
 	if (curTime > timeLength)
		curTime = 0.0f;
	Skeleton::Bone *rootBone = skeleton.GetBone("root");

	for (int i = 0; i < tracks.size(); i++)
	{
		Track &t = tracks[i];
		vector<KeyFrame*> twoKeyFrame = findTwoKeyframes(curTime, t);
		KeyFrame *leftFrame = twoKeyFrame[0];
		KeyFrame *rightFrame = twoKeyFrame[1];

		//KeyFrame *leftFrame = &t.keyFrames[0];
		//KeyFrame *rightFrame = &t.keyFrames[0];

		if (t.BoneName == "Root")
			int i = 0;

		if (leftFrame != NULL && rightFrame != NULL)
		{
			if (leftFrame != curLeftFrame || rightFrame != curLeftFrame)
			{
				if (leftFrame == rightFrame)
					computePosMatrix(leftFrame, skeleton.GetBone(t.BoneName));
				else
					computePosMatrix(curTime, leftFrame, rightFrame, skeleton.GetBone(t.BoneName));

				curLeftFrame = leftFrame;
				curRightFrame = rightFrame;
				dataChanged = true;
			}
		}
	}
	updateAllMatrix();
}


void Animation::updateAllMatrix()
{
	Skeleton::Bone *rootBone = skeleton.GetBone("root");
	FILE *fp = fopen("a.txt", "w");
	rootBone->updateTransform(fp);


	for (int i = 0; i < skeleton.GetBones().size(); i++)
	{
		fprintf(fp, "%s\n", skeleton.GetBone(i)->name.c_str());
		MathUntil::printfMatrix(skeleton.GetBone(i)->inverseMatrix, fp);
	}
	fclose(fp);
	rootBone->computePosMatrix();
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



void Animation::computePosMatrix(float time, KeyFrame *leftFrame, KeyFrame *rightFrame, Skeleton::Bone *b)
{
	XMFLOAT3 translate = MathUntil::lerpFloat3(leftFrame->translate, rightFrame->translate, leftFrame->startTime, time, rightFrame->startTime);
	
	XMVECTOR leftQ = XMLoadFloat4(&leftFrame->quaternion);
	XMVECTOR rightQ = XMLoadFloat4(&rightFrame->quaternion);

	XMVECTOR quaternion = XMQuaternionSlerp(leftQ, rightQ, (time - leftFrame->startTime) / (rightFrame->startTime - leftFrame->startTime));

	XMMATRIX m2 = XMMatrixRotationQuaternion(quaternion);
	XMMATRIX m3 = XMMatrixTranslation(translate.x, translate.y, translate.z);

	XMStoreFloat4x4(&b->localMatrix, m2*m3);
}

void Animation::computePosMatrix(KeyFrame *frame, Skeleton::Bone *b)
{
	XMMATRIX m2 = XMMatrixRotationQuaternion(XMLoadFloat4(&frame->quaternion));
	XMMATRIX m3 = XMMatrixTranslation(frame->translate.x, frame->translate.y, frame->translate.z);

	XMStoreFloat4x4(&b->localMatrix, m2*m3);
}

vector<Animation::KeyFrame*> Animation::findTwoKeyframes(float time, Track &t)
{
	KeyFrame *leftFrame = NULL;
	KeyFrame *rightFrame = NULL;

	for (int i = 0; i < t.keyFrames.size(); i++)
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




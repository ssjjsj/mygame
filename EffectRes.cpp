#include "effect.h"
#include "global.h"
#include "tinyxml/tinystr.h"
#include "tinyxml/tinyxml.h"

EffectRes::EffectRes()
{

}

EffectRes::EffectRes(string path)
{
	createEffect(path);
}


EffectRes::~EffectRes()
{

}

void EffectRes::createEffect(string path)
{
	path = ShaderPath + path;

	TiXmlDocument doc = TiXmlDocument(path.c_str());
	doc.LoadFile();

	TiXmlNode *effectRootNode = doc.RootElement();

	TiXmlNode *shaderRootNode = effectRootNode->FirstChild();

	for (TiXmlNode *curShaderNode = shaderRootNode->FirstChild(); curShaderNode != NULL; curShaderNode = curShaderNode->NextSibling())
	{
		TiXmlElement *shaderElement = (TiXmlElement*)curShaderNode;
		ShaderResource *res = new ShaderResource();
		res->loadShader(shaderElement);
		shaderRes.push_back(res);
	}

	int i = 3;
	//TiXmlNode *techRootNode = shaderRootNode->NextSibling();
	//for (TiXmlNode *curTech = techRootNode->FirstChild(); curTech != NULL; curTech = curTech->NextSibling())
	//{
	//	
	//}
}
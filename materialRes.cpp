#include "materialRes.h"
#include "global.h"
#include "tinyxml\tinystr.h"
#include "tinyxml\tinyxml.h"
#include "shaderResource.h"

MaterialRes::MaterialRes(string name)
{
	load(name);
}

MaterialRes::~MaterialRes()
{
}


void MaterialRes::load(string name)
{
	string path = MaterialPath + name;
	TiXmlDocument doc = TiXmlDocument(path.c_str());
	doc.LoadFile();

	TiXmlNode *root = doc.RootElement();

	for (TiXmlNode *curMaterialDataNode = root->FirstChild(); curMaterialDataNode != NULL; curMaterialDataNode = curMaterialDataNode->NextSibling())
	{
		TiXmlElement *materialElement = (TiXmlElement*)(curMaterialDataNode);
		string materialName = materialElement->Attribute("name");

		MaterialData data;

		TiXmlElement *shaderNode = (TiXmlElement*)materialElement->FirstChild();
		string shaderName = shaderNode->Attribute("name");

		data.res = shared_ptr<ShaderResource>(new ShaderResource(shaderName));

		TiXmlNode *texRootNode = shaderNode->NextSibling();
		for (TiXmlNode *curTexNode = texRootNode->FirstChild(); curTexNode != NULL; curTexNode = curTexNode->NextSibling())
		{
			TiXmlElement *texNode = (TiXmlElement*)curTexNode;
			data.texName.push_back(texNode->Attribute("name"));
		}

		dataMap[materialName] = data;
	}
}
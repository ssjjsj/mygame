#include "materialRes.h"
#include "global.h"
#include "tinyxml\tinystr.h"
#include "tinyxml\tinyxml.h"

MaterialRes::MaterialRes(string name)
{

}

MaterialRes::~MaterialRes()
{
	for (map<string, MaterialData>::iterator it = dataMap.begin(); it != dataMap.end(); it++)
	{
		delete it->second.res;
	}

	dataMap.clear();
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

		data.res = new ShaderResource(name);

		TiXmlElement *texNode = (TiXmlElement*)shaderNode->NextSibling();
		data.texName = texNode->Attribute("name");

		dataMap[materialName] = data;
	}
}
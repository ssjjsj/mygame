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

		TiXmlElement *ambientNode = (TiXmlElement*)materialElement->FirstChild();
		data.ambient.x = atof(ambientNode->Attribute("r"));
		data.ambient.y = atof(ambientNode->Attribute("g"));
		data.ambient.z = atof(ambientNode->Attribute("b"));
		TiXmlElement *diffuseNode = (TiXmlElement*)ambientNode->NextSibling();
		data.diffuse.x = atof(diffuseNode->Attribute("r"));
		data.diffuse.y = atof(diffuseNode->Attribute("g"));
		data.diffuse.z = atof(diffuseNode->Attribute("b"));
		TiXmlElement *specularNode = (TiXmlElement*)diffuseNode->NextSibling();
		data.specular.x = atof(specularNode->Attribute("r"));
		data.specular.y = atof(specularNode->Attribute("g"));
		data.specular.z = atof(specularNode->Attribute("b"));
		
		TiXmlElement *shaderNode = (TiXmlElement*)specularNode->NextSibling();
		string shaderName = shaderNode->Attribute("name");

		data.res = new ShaderResource(shaderName);

		TiXmlNode *texRootNode = shaderNode->NextSibling();
		for (TiXmlNode *curTexNode = texRootNode->FirstChild(); curTexNode != NULL; curTexNode = curTexNode->NextSibling())
		{
			TiXmlElement *texNode = (TiXmlElement*)curTexNode;
			data.texName.push_back(texNode->Attribute("name"));
		}

		dataMap[materialName] = data;
	}
}
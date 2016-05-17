#include "material.h"

Material::Material()
{

}


Material::Material(MaterialRes::MaterialData &data)
{
	ambient = data.ambient;
	specular = data.specular;
	diffuse = data.diffuse;
	
	Ptr<EffectRes> res = data.res;
	for (int i = 0; i < res->getShaderResCount(); i++)
		shaders.push_back(res->getShaderRes(i)->createInstance());

	if (!data.texName.empty())
	{
		for (int i = 0; i < data.texName.size(); i++)
		{
			Texture *tex = new Texture(data.texName[i]);
			textures.push_back(tex);
		}
	}
}


Material::~Material()
{
	for (int i = 0; i < shaders.size(); i++)
		delete shaders[i];
	for (int i = 0; i < textures.size(); i++)
	{
		Texture *tex = textures[i];
		delete tex;
	}
}


void Material::addTexture(Texture *tex)
{
	textures.push_back(tex);
}


void Material::addShader(Shader *s)
{
	shaders.push_back(s);
}


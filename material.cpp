#include "material.h"

Material::Material()
{

}


Material::Material(MaterialRes::MaterialData &data)
{
	ambient = data.ambient;
	specular = data.specular;
	diffuse = data.diffuse;
	
	shader = new Shader(data.res.get());

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
	delete shader;
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


void Material::setShader(Shader *s)
{
	shader = s;
}


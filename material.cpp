#include "material.h"

Material::Material()
{

}


Material::Material(MaterialRes::MaterialData &data)
{
	shader = new Shader(data.res.get());

	Texture *tex = new Texture(data.texName);
	textures.push_back(tex);
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


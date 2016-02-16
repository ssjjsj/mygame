#ifndef MATERIAL_H
#define MATERIAL_H

#include "shader.h"
#include "Texture.h"

class Material
{
public:
	Material();
	~Material();

private:
	Shader shader;
	vector<Texture> textures;

public:
	Shader& getShader() { return shader; }
	vector<Texture>& getTextures() { return textures; }
};
#endif
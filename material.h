#ifndef MATERIAL_H
#define MATERIAL_H

#include "shader.h"
#include "Texture.h"
#include "materialRes.h"
#include "MathHelp.h"

#include <vector>
using namespace std;

class Material
{
public:
	Material();
	Material(MaterialRes::MaterialData &data);
	~Material();

private:
	Shader *shader;
	vector<Texture*> textures;
	XMFLOAT3 ambient;
	XMFLOAT3 diffuse;
	XMFLOAT3 specular;

public:
	void setShader(Shader *shader);
	void addTexture(Texture *tex);
	Shader* getShader() { return shader; };
	vector<Texture*>& getTextures() { return textures; };
	XMFLOAT3 getAmbient() { return ambient; }
	XMFLOAT3 getDiffuse() { return diffuse; }
	XMFLOAT3 getSpecular() { return specular; }
};
#endif
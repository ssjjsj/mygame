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
	vector<Shader*> shaders;
	vector<Texture*> textures;
	XMFLOAT3 ambient;
	XMFLOAT3 diffuse;
	XMFLOAT3 specular;

public:
	void addShader(Shader *shader);
	void addTexture(Texture *tex);
	Shader* getShader(int i) { return shaders[i]; };
	vector<Texture*>& getTextures() { return textures; };
	XMFLOAT3 getAmbient() { return ambient; }
	XMFLOAT3 getDiffuse() { return diffuse; }
	XMFLOAT3 getSpecular() { return specular; }
};
#endif
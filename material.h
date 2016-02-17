#ifndef MATERIAL_H
#define MATERIAL_H

#include "shader.h"
#include "Texture.h"

#include <vector>
using namespace std;

class Material
{
public:
	Material();
	~Material();

private:
	Shader *shader;
	vector<Texture*> textures;

public:
	void setShader(Shader *shader);
	void addTexture(Texture *tex);
	Shader* getShader() { return shader; };
	vector<Texture*>& getTextures() { return textures; };
};
#endif
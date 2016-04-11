#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "global.h"
#include <D3D11.h>
using namespace std;

class Texture
{
public:
	Texture() {};
	Texture(string texName);
	~Texture();
	Texture(ID3D11ShaderResourceView *tex);

	ID3D11ShaderResourceView* getTexture() { return texture; }

private:
	void createTexture(string name);

private:
	ID3D11ShaderResourceView* texture;
};

#endif
#pragma once
#ifndef GSPRITE_H
#define GSPRITE_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include <DirectXMath.h>
#include "GTexture.h"

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;

//--------------------------------------------------------------------------------------
//Class GSprite: represent a sprite. A rectangle with a color texture
//--------------------------------------------------------------------------------------
class GSprite
{
	struct VertexData
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};
public:
	GSprite(void);
	~GSprite(void)											{}

	void shutDown();
	bool initialize(ID3D11Device* device, int screenWidth, int screenHeight,const char* textureFileName,
							int spriteWidth, int spriteHeight);

	bool render(ID3D11DeviceContext* deviceContext, int posX, int posY);
	
	inline int getIndexCount()									{return _indexCount;}
	inline ID3D11ShaderResourceView* getTexture()				{return _pTexture->getTexture();}

private:
	bool initializeBuffers(ID3D11Device* device);
	void shutDownBuffers();
	bool updateBuffers(ID3D11DeviceContext* devContext, int positionX, int positionY);
	void renderBuffers(ID3D11DeviceContext* devContext);

	bool loadTexture(ID3D11Device* device,const char* fileName);
	void releaseTexture();

	ID3D11Buffer *_pVertexBuffer, *_pIndexBuffer;
	GTexture* _pTexture;
	int _vertexCount, _indexCount;
	int _width, _height;
	int _prevPosX, _prevPosY;
	int _screenWidth, _screenHeight;

	//prevent copy
	GSprite(const GSprite&);
	GSprite& operator=(const GSprite&);
};

#endif		//GSprite


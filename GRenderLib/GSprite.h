#pragma once
#ifndef GSPRITE_H
#define GSPRITE_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include <DirectXMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <GMath.h>
#include "GTexture.h"

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
using DirectX::SpriteFont;
using DirectX::SpriteBatch;
using DirectX::SpriteEffects;
using GMath::GVector2;
using GMath::Radians;

//--------------------------------------------------------------------------------------
//Class GSprite: represent a sprite. A rectangle with a color texture
//--------------------------------------------------------------------------------------
class GSprite
{
public:
	GSprite(void);
	~GSprite(void)												{}

	void shutDown();
	bool initialize(ID3D11Device* device, int screenWidth, int screenHeight,const char* textureFileName);

	bool render(_In_ SpriteBatch* spriteBatch, float posX, float posY, float scale=1.0f, float rotation=.0f);
	
	inline ID3D11ShaderResourceView* getTexture()				{return _pTexture->getTexture();}

private:
	bool loadTexture(ID3D11Device* device,const char* fileName);
	void releaseTexture();

	GTexture* _pTexture;
	int _screenWidth, _screenHeight;

	//prevent copy
	GSprite(const GSprite&);
	GSprite& operator=(const GSprite&);
};

#endif		//GSprite


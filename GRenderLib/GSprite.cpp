#include "GRenderIncludes.h"
#include "GSprite.h"

GSprite::GSprite(void) : _pTexture(nullptr)
{
}

void GSprite::shutDown()
{
	releaseTexture();
}

bool GSprite::initialize(ID3D11Device* device, int screenWidth, int screenHeight,const char* textureFileName)
{
	_screenWidth= screenWidth;			_screenHeight=screenHeight;

	bool result = loadTexture(device,textureFileName);
	if(!result)
	{
		DBG_OUTPT("GSprite - cannot load texture "<< textureFileName );
		return false;
	}
	return true;
}

bool GSprite::render(_In_ SpriteBatch* spriteBatch, float positionX, float positionY, float scale, float rotation)
{
	spriteBatch->Draw(_pTexture->getTexture(), XMFLOAT2(positionX, positionY), nullptr, DirectX::Colors::White, rotation, XMFLOAT2(.0f, .0f), scale);

	return true;
}

bool GSprite::loadTexture(ID3D11Device* device, const char* fileName)
{
	_pTexture = new GTexture();
	if(_pTexture == nullptr)				{return false;}

	bool result = _pTexture->initialize(device,fileName);
	if(!result)
	{
		DBG_OUTPT("GSprite::loadTexture - failed initialize texture with file " << fileName);
		return false;
	}
	return true;
}

void GSprite::releaseTexture()
{
	if(_pTexture!=nullptr)
	{
		_pTexture->shutDown();
		delete _pTexture;
		_pTexture = nullptr;
	}
}
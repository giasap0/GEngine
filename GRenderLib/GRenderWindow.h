#pragma once
#ifndef GRENDERWINDOW_H
#define GRENDERWINDOW_H
//works like GSprite

#include <d3d11.h>
#include <DirectXMath.h>
#include <GMathCore.h>

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
using GMath::Int32;

//-----------------------------------------------------------------------------------------------------------
//Class GRenderWindow: use with GRenderToTexture to render a scene in a window and paint it upon the main scene
//-----------------------------------------------------------------------------------------------------------
class GRenderWindow
{
private:
	struct VertexData
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};
public:
	GRenderWindow(void);
	virtual ~GRenderWindow(void)			{}

	void shutDown();
	bool initialize(ID3D11Device* device, int screenWidth, int screenHeight, int spriteWidth, int spriteHeight);

	bool render(ID3D11DeviceContext* deviceContext, int posX, int posY);
	
	inline Int32 getIndexCount() const					{return _indexCount;}
	inline Int32 getWindowWidth() const					{return _width;}
	inline Int32 getWindowHeight() const				{return _height;}

private:
	bool initializeBuffers(ID3D11Device* device);
	void shutDownBuffers();
	bool updateBuffers(ID3D11DeviceContext* devContext, int positionX, int positionY);
	void renderBuffers(ID3D11DeviceContext* devContext);

	ID3D11Buffer *_pVertexBuffer, *_pIndexBuffer;
	Int32 _vertexCount, _indexCount;
	Int32 _screenWidth, _screenHeight;
	Int32 _width, _height;
	Int32 _prevPosX, _prevPosY;

	//prevent copy
	GRenderWindow(const GRenderWindow&);
	GRenderWindow& operator= ( const GRenderWindow&);
};

#endif		//GRENDERWINDOW_H



/*

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
	GSprite(const GSprite&)									{}
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
};

*/
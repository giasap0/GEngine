#pragma once
#ifndef GTEXTURE_H
#define GTEXTURE_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include <d3d11.h>

//--------------------------------------------------------------------------------------
//Class GTexture: maintain a texture in memory
//--------------------------------------------------------------------------------------
class GTexture
{
public:
	GTexture(void);
	GTexture(const GTexture&)								{}
	~GTexture(void)											{}

	void shutDown();
	bool initialize(ID3D11Device* device, const char* fileName);

	inline ID3D11ShaderResourceView* getTexture()			{return _pTexture;}

private:
	ID3D11ShaderResourceView*	_pTexture;
};

#endif	//GTEXTURE_H
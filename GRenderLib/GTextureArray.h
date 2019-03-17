#pragma once
#ifndef GTEXTUREARRAY_H
#define GTEXTUREARRAY_H

#include <d3d11.h>
#include <GMathCore.h>
#include <GEngineCore.h>

//----------------------------------------------------------------------------------------------------------------------
//Class GTextureArray: Instead of GTexture can have multiple textures and give calling objects access to those textures.
//----------------------------------------------------------------------------------------------------------------------
class GTextureArray
{
public:
	GTextureArray(void);
	virtual ~GTextureArray(void)			{}

	//bool initialize(ID3D11Device* device, const char* fileName1, const char* fileName2);
	bool initialize(ID3D11Device* device, const GStringList& fileNames);
	void shutDown();

	inline ID3D11ShaderResourceView** getTextureArray()		{return _pTextures;}
	inline GMath::uint32 getSize()							{return _size;}

private:
	ID3D11ShaderResourceView** _pTextures;
	GMath::uint32 _size;

	//prevent copy
	GTextureArray(const GTextureArray&);
	GTextureArray& operator = (const GTextureArray&);
};

#endif		//GTEXTUREARRAY_H

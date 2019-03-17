#pragma once
#ifndef GMULTITEXTURE_SHADER_H
#define GMULTITEXTURE_SHADER_H

#include "GAbstract2TexturesShader.h"

//--------------------------------------------------
//Class GMultiTextureShader:  blend 2 color textures
//--------------------------------------------------
class GMultiTextureShader : public GAbstract2TexturesShader
{
public:
	GMultiTextureShader(void);
	virtual ~GMultiTextureShader(void)								{}

	virtual bool initialize(ID3D11Device* device, HWND hWnd);

private:

	//prevent copy
	GMultiTextureShader(const GMultiTextureShader&);
	GMultiTextureShader& operator=(const GMultiTextureShader&);
};

#endif		//GMULTITEXTURE_SHADER_H

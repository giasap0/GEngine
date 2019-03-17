#include "GMultiTextureShader.h"

GMultiTextureShader::GMultiTextureShader(void) : GAbstract2TexturesShader()
{
}

bool GMultiTextureShader::initialize(ID3D11Device* device, HWND hWnd)
{
	bool result = initializeShader(device, hWnd, "shaders/multitexture.vs","shaders/multitexture.ps");
	if(!result)				{return false;}

	return true;
}

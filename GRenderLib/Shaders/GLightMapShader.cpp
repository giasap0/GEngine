#include "GLightMapShader.h"

GLightMapShader::GLightMapShader(void) : GAbstract2TexturesShader()
{
}


bool GLightMapShader::initialize(ID3D11Device* device, HWND hWnd)
{
	bool result = initializeShader(device, hWnd, "shaders/lightmap.vs","shaders/lightmap.ps");
	if(!result)				{return false;}

	return true;
}

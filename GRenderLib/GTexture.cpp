#include "GRenderIncludes.h"
#include "GTexture.h"
#include <DDSTextureLoader.h>


GTexture::GTexture(void) : _pTexture(nullptr)
{
}


void GTexture::shutDown()
{
	if(_pTexture!= nullptr)
	{
		_pTexture->Release();
		_pTexture = nullptr;
	}
}

bool GTexture::initialize(ID3D11Device* device, const char* fileName)
{
	size_t convertedChars = 0;
	size_t origsize = strlen(fileName) + 1;
	wchar_t* wFileName = new wchar_t[origsize];
	mbstowcs_s(&convertedChars,wFileName,origsize,fileName,origsize);
	
	HRESULT result = DirectX::CreateDDSTextureFromFile(device,wFileName,nullptr,&_pTexture);

	delete[] wFileName;

	if(FAILED(result))
	{
		DBG_OUTPT("GTexture::initialize Failed loading texture from file " << fileName );
		DBG_OUTPT("error HRESULT = " << result );
		return false;
	}
	return true;
}
	//HRESULT result = D3DX11CreateShaderResourceViewFromFileA(device,fileName,nullptr,nullptr,&_pTexture,nullptr);
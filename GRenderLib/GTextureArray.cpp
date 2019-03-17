#include "GTextureArray.h"
#include "GRenderIncludes.h"
#include <DDSTextureLoader.h>


GTextureArray::GTextureArray(void) : _pTextures(nullptr)
{
}

void GTextureArray::shutDown()
{
	if(_pTextures != nullptr)
	{
		for (int i=_size-1; i>=0; i--)
		{
			if(_pTextures[i] != nullptr)
			{
				_pTextures[i]->Release();
				_pTextures[i] = nullptr;
			}
		}
		delete _pTextures;
		_pTextures = nullptr;
	}
}

bool GTextureArray::initialize(ID3D11Device* device, const GStringList& fileNames)
{
	if(_pTextures != nullptr)
	{
		DBG_OUTPT("GTextureArray::initializeTextures - textures array is not empty. Deleting all data");
		this->shutDown();
	}
	_size = fileNames.size();
	_pTextures = new ID3D11ShaderResourceView*[_size];
	if(_pTextures == nullptr)		{return false;}

	size_t stringSize, convertedChars;
	wchar_t* wFileName = nullptr;
	HRESULT result = S_OK;

	for(GMath::uint32 i=0; i<_size; i++)
	{
		stringSize = fileNames.at(i).size()+1;
		wFileName = new wchar_t[stringSize];
		convertedChars = 0;
		mbstowcs_s(&convertedChars,wFileName,stringSize,fileNames.at(i).const_data(),stringSize);

		result = DirectX::CreateDDSTextureFromFile(device,wFileName,nullptr,&_pTextures[i]);
		delete[] wFileName;
		wFileName = nullptr;
		if( FAILED (result) )
		{
			DBG_OUTPT("GTextureArray::initialize Failed loading texture from file " << fileNames.at(i).const_data() );
			DBG_OUTPT("stopped loading at file n°" << i+1 << " , error HRESULT = " << result);
			return false;
		}

	}	//end for
	return true;
}


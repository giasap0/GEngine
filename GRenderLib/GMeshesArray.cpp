#include "GMeshesArray.h"
#include <DDSTextureLoader.h>

//****************************************  GMeshPart  ****************************************//

bool GMeshPart::initialize(ID3D11Device* device, const GString& colorTexture_fileName, const GString& normalMap_fileName)
{
	bool res = initializeTextures(device, colorTexture_fileName, normalMap_fileName);

	return res;
}

bool GMeshPart::initializeTextures(ID3D11Device* device, const GString& colorTexture_fileName, const GString& normalMap_fileName)
{
	if(_pTexturesArray != nullptr)
	{
		DBG_OUTPT("GMeshPart::initializeTextures - textures array is not empty. Deleting all data");
		this->shutDown();
	}
	_pTexturesArray = new ID3D11ShaderResourceView*[2];
	if(_pTexturesArray == nullptr)		{return false;}

	size_t convertedChars = 0;
	size_t origsize = colorTexture_fileName.size()+1;
	wchar_t* wFileName = nullptr;
	HRESULT result = S_OK;

	if(origsize>1)
	{
		wFileName = new wchar_t[origsize];
		mbstowcs_s(&convertedChars,wFileName,origsize, colorTexture_fileName.const_data(),origsize);
		result = DirectX::CreateDDSTextureFromFile(device,wFileName,nullptr, &_pTexturesArray[0]);

		delete[] wFileName;
		wFileName = nullptr;
		if(FAILED(result))
		{
			DBG_OUTPT("GMeshPart::initializeTextures Failed loading color texture from file " << colorTexture_fileName.const_data() );
			DBG_OUTPT("error HRESULT = " << result );
			return false;
		}
	}
	
	convertedChars = 0;		origsize = normalMap_fileName.size()+1;
	if(origsize>1)
	{
		wFileName = new wchar_t[origsize];
		mbstowcs_s(&convertedChars,wFileName,origsize, normalMap_fileName.const_data(),origsize);

		result = DirectX::CreateDDSTextureFromFile(device,wFileName,nullptr, & _pTexturesArray[1]);

		delete[] wFileName;
		wFileName = nullptr;
		if(FAILED(result))
		{
			DBG_OUTPT("GMeshPart::initializeTextures Failed loading normal map from file " << normalMap_fileName.const_data() );
			DBG_OUTPT("error HRESULT = " << result );
			return false;
		}
	}

	return true;
}


void GMeshPart::shutDown()
{
	if(_pTexturesArray != nullptr)
	{
		if(_pTexturesArray[1] != nullptr)
		{
			_pTexturesArray[1]->Release();
			_pTexturesArray[1] = nullptr;
		}
		if(_pTexturesArray[0] != nullptr)
		{
			_pTexturesArray[0]->Release();
			_pTexturesArray[0] = nullptr;
		}
		delete _pTexturesArray;
		_pTexturesArray = nullptr;
	}

	_alpha = _shininess = 0.0f;
	_specular.setValues(.0f,.0f,.0f,.0f);
	_ambient = _diffuse = _specular;
	_startIndex = _indexCount = 0;
}


//****************************************  GMeshArray  ****************************************//

GMeshesArray::GMeshesArray(void) : _meshParts( GVector<GMeshPart>() )
{
}

void GMeshesArray::shutDown()
{
	uint top = _meshParts.size()-1;

	DBG_OUTPT("GMeshesArray::shutDown() top = " << top)

	for(uint i=top; i>=0; i--)
	{
		if(i==-1)
			break;
		DBG_OUTPT("i = " << i)
		_meshParts[i].shutDown();
	}

	_meshParts.clear();
	return;
}

bool GMeshesArray::initialize(ID3D11Device* device, const GVector<ModelsStructs::Mesh>& meshes)
{
	if( _meshParts.size()>0)
	{
		DBG_OUTPT("GMeshesArray::initialize - meshes vector is not empty. Clearing all elements");
		shutDown();
	}
	uint meshCount = meshes.size();
	_meshParts.resize( meshCount );
	_meshParts.squeeze();

	bool result=true;

	DBG_OUTPT("GMeshesArray::initialize - creating " << meshCount <<" meshes");
	for(uint i=0; i<meshCount; i++)
	{
		const ModelsStructs::Material* mate = meshes[i].pMaterial;

		result = _meshParts[i].initialize(device, mate->colorMap_fileName, mate->normalMap_fileName);

		_meshParts[i].setStartIndex( meshes[i].startIndex );
		_meshParts[i].setIndexCount( meshes[i].triangleCount*3 );
		_meshParts[i].setAmbient( mate->ambient );
		_meshParts[i].setDiffuse( mate->diffuse );
		_meshParts[i].setSpecular( mate->specular );
		_meshParts[i].setShininess( mate->shininess );
		_meshParts[i].setAlpha ( mate->alpha );

		if( !result)
		{
			DBG_OUTPT("GMeshesArray::initialize - an error occurred. Cannot initialize. Stopped at mesh n "<<i);
			return false;
		}
	}	//EO for(uint i=0; i<meshCount; i++)

	return true;
}

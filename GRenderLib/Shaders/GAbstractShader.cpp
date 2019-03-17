#include "GAbstractShader.h"
#include <D3Dcompiler.h>
#include <cstdlib>


GAbstractShader::GAbstractShader() :   _pVertexShader(nullptr), _pPixelShader(nullptr),
										_pLayout(nullptr), _pMatrixBuffer(nullptr)
{
}

void GAbstractShader::shutDown()
{
	unloadContent();
	if(_pMatrixBuffer!=nullptr)
	{
		_pMatrixBuffer->Release();
		_pMatrixBuffer = nullptr;
	}

	if(_pLayout!=nullptr)
	{
		_pLayout->Release();
		_pLayout = nullptr;
	}

	if(_pPixelShader!=nullptr)
	{
		_pPixelShader->Release();
		_pPixelShader = nullptr;
	}

	if(_pVertexShader!=nullptr)
	{
		_pVertexShader->Release();
		_pVertexShader = nullptr;
	}
}

												// protected
//--------------------------------------------------------------------------------------
// Compile a vertex shader from file
//-------------------------------------------------------------------------------------
HRESULT GAbstractShader::compileD3DShader(char* filePath, char* entry, char* shaderModel, ID3DBlob** buffer, HWND hWnd) const
{
	//DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
	
	#if defined( DEBUG ) || defined( _DEBUG )
		shaderFlags |= D3DCOMPILE_DEBUG;
	#endif

	ID3DBlob* errorBuffer = nullptr;

	wchar_t* wfilePath = new wchar_t[MAX_FILENAME_LENGTH];
	size_t convertedChars = 0;
	size_t origsize = strlen(filePath) + 1;
	mbstowcs_s(&convertedChars,wfilePath,origsize,filePath,MAX_FILENAME_LENGTH);

	HRESULT result = D3DCompileFromFile(wfilePath,nullptr,nullptr,entry,shaderModel,shaderFlags,0,buffer,&errorBuffer);

	delete[] wfilePath;
	wfilePath = nullptr;

	if( FAILED( result ) )
	{
		if( errorBuffer!= 0)
		{
			outputShaderErrorMessage(errorBuffer,hWnd,filePath);
			errorBuffer->Release();
		}
		else
		{
			MessageBoxA(hWnd,filePath,"Missing Shader File ",MB_OK);
		}
		return result;
	}
	if ( errorBuffer != nullptr )
	{
		errorBuffer->Release();
	}
	return result;
}
void GAbstractShader::outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hWnd, char* fileName) const
{
	std::ofstream fout;

	// Get a pointer to the error message text buffer.
	char* compileErrors = (char*)(errorMessage->GetBufferPointer());

	unsigned int bufferSize = errorMessage->GetBufferSize();

	fout.open("shaderError.txt");

	// Write out the error message.
	DBG_OUTPT("error compiling shader " << fileName);
	DBG_OUTPT("" << compileErrors );
	for(unsigned int i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();
	errorMessage->Release();
	errorMessage = nullptr;
	
	MessageBoxA(hWnd,"Error compiling shader. Check shaderError.txt for message.",fileName,MB_OK);
}

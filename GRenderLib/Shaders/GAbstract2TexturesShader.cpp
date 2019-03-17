#include "GAbstract2TexturesShader.h"

GAbstract2TexturesShader::GAbstract2TexturesShader(void) : GAbstractShader(), _pSampleState(nullptr)
{
}

bool GAbstract2TexturesShader::render(ID3D11DeviceContext* deviceContext, int indexCount,
							const GMatrix4x4& world, const GMatrix4x4& view, const GMatrix4x4& projection,
							ID3D11ShaderResourceView** texturesPtr)
{
	bool result = setShaderParameters(deviceContext,world,view,projection,texturesPtr);
	if(!result)				{return false;}
	renderShader(deviceContext,indexCount);
	return true;
}

												// protected
void GAbstract2TexturesShader::unloadContent()
{
	if(_pSampleState != nullptr)
	{
		_pSampleState->Release();
		_pSampleState = nullptr;
	}
}

bool GAbstract2TexturesShader::initializeShader(ID3D11Device* device, HWND hWnd, char* vsFileName, char* psFileName)
{
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	//compile vertex shader
	HRESULT result = compileD3DShader(vsFileName,"VS_Main","vs_5_0",&vertexShaderBuffer,hWnd);
	if( FAILED (result) )				{return false;}
	//compile pixel shader
	result = compileD3DShader(psFileName,"PS_Main","ps_5_0",&pixelShaderBuffer,hWnd);
	if(FAILED(result) )					{return false;}
	// Create the vertex shader from the buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
										nullptr, &_pVertexShader);
	if(FAILED(result))
	{
		DBG_OUTPT("GAbstract2TexturesShader::initializeShader - error creating vertex shader from file " << vsFileName);
		DBG_OUTPT("error HRESULT = " << result);
		return false;
	}
	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(),
										nullptr, &_pPixelShader);
	if(FAILED(result))
	{
		DBG_OUTPT("GAbstract2TexturesShader::initializeShader - error creating pixel shader from file " << psFileName);
		DBG_OUTPT("error HRESULT = " << result);
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the GModel class and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), 
										vertexShaderBuffer->GetBufferSize(), &_pLayout);
	if(FAILED(result))
	{
		DBG_OUTPT("GAbstract2TexturesShader::initializeShader - error creating input layout HRESULT= " << result);
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	// Setup the description of the matrix dynamic constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, nullptr, &_pMatrixBuffer);
	if(FAILED(result))
	{
		DBG_OUTPT("GAbstract2TexturesShader::initializeShader - error creating matrix buffer HRESULT= " << result);
		return false;
	}

	// Create a texture sampler state and description.
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &_pSampleState);
	if(FAILED(result))
	{
		DBG_OUTPT("GAbstract2TexturesShader::initializeShader - error creating sampler state HRESULT= " << result);
		return false;
	}
	return true;
}

bool GAbstract2TexturesShader::setShaderParameters(ID3D11DeviceContext* deviceContext,
											const GMatrix4x4& world, const GMatrix4x4& view, const GMatrix4x4& projection,
											ID3D11ShaderResourceView** texturesPtr)
{
	// Transpose the matrices to prepare them for the shader.
	XMMATRIX worldM = XMMatrixTranspose( world.toXMMatrix() );
	XMMATRIX viewM = XMMatrixTranspose( view.toXMMatrix() );
	XMMATRIX projectionM = XMMatrixTranspose( projection.toXMMatrix() );

	// Lock the constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = deviceContext->Map(_pMatrixBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource);
	if(FAILED(result))
	{
		DBG_OUTPT("GAbstract2TexturesShader::setShaderParameters - error mapping subresource HRESULT= " << result);
		return false;
	}
	MatrixBufferType* dataPtr= (MatrixBufferType*)mappedResource.pData;
	// Copy the matrices into the constant buffer.
	XMStoreFloat4x4( &dataPtr->world, worldM);
	XMStoreFloat4x4( &dataPtr->view, viewM);
	XMStoreFloat4x4( &dataPtr->projection, projectionM);
	deviceContext->Unmap(_pMatrixBuffer, 0);					// Unlock the constant buffer
	unsigned int bufferNumber = 0;								//Set the position of the constant buffer in the vertex shader
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_pMatrixBuffer);	//set the constant buffer in the vertex shader
	
	deviceContext->PSSetShaderResources(0,2,texturesPtr);		//set shader texture array resource in pixel shader
	return true;
}

void GAbstract2TexturesShader::renderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(_pLayout);

	// Set the vertex and pixel shaders
	deviceContext->VSSetShader(_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(_pPixelShader, NULL, 0);
	// Set the sampler stat in the pixel shader
	deviceContext->PSSetSamplers(0,1,&_pSampleState);

	// Render 
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}

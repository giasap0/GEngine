#include "GNormalMapShader.h"

GNormalMapShader::GNormalMapShader() : GAbstractShader(), _pSampleState(nullptr), _pLightBuffer(nullptr)
{
}

bool GNormalMapShader::initialize(ID3D11Device* device, HWND hWnd)
{
	bool result = initializeShader(device,hWnd,"shaders/normalmap.vs","shaders/normalmap.ps");
	if(!result)
	{
		DBG_OUTPT("Failed to initialize Normal Map Shader");
		return false;
	}
	return true;
}

bool GNormalMapShader::render(ID3D11DeviceContext* deviceContext, int indexCount,
						const GMatrix4x4& world, const GMatrix4x4& view, const GMatrix4x4& projection,
						ID3D11ShaderResourceView** texturesPtr,
						const GVector3& lightDirection, const GColor& diffuseColor, const GColor& ambientColor)
{
	bool result = setShaderParameters(deviceContext,world,view,projection,
										texturesPtr,lightDirection,diffuseColor,ambientColor);
	if(!result)				{return false;}

	renderShader(deviceContext,indexCount);
	return true;
}
												// protected
void GNormalMapShader::unloadContent()
{
	if(_pLightBuffer != nullptr)
	{
		_pLightBuffer->Release();
		_pLightBuffer = nullptr;
	}
	if(_pSampleState != nullptr)
	{
		_pSampleState->Release();
		_pSampleState = nullptr;
	}
}

bool GNormalMapShader::initializeShader(ID3D11Device* device, HWND hWnd, char* vsFileName, char* psFileName)
{
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	//compile vertex shader
	HRESULT result = compileD3DShader(vsFileName,"VS_Main","vs_5_0",&vertexShaderBuffer,hWnd);
	if( FAILED (result) )
	{
		return false;
	}
	//compile pixel shader
	result = compileD3DShader(psFileName,"PS_Main","ps_5_0",&pixelShaderBuffer,hWnd);
	if(FAILED(result) )
	{
		return false;
	}
	// Create the vertex shader from the buffer
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
										nullptr, &_pVertexShader);
	if(FAILED(result))
	{
		DBG_OUTPT("GNormalMapShader::initializeShader - error creating vertex shader HRESULT= " << result);
		return false;
	}
	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(),
										nullptr, &_pPixelShader);
	if(FAILED(result))
	{
		DBG_OUTPT("GNormalMapShader::initializeShader - error creating pixel shader HRESULT= " << result);
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	// Now setup the layout of the data that goes into the shader.
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

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "BINORMAL";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;

	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), 
										vertexShaderBuffer->GetBufferSize(), &_pLayout);
	if(FAILED(result))
	{
		DBG_OUTPT("GNormalMapShader::initializeShader - error creating vertex input layout HRESULT= " << result);
		return false;
	}
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &_pMatrixBuffer);
	if(FAILED(result))
	{
		DBG_OUTPT("GNormalMapShader::initializeShader - error creating matrix buffer HRESULT= " << result);
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
		DBG_OUTPT("GNormalMapShader::initializeShader - error creating texture sampler state HRESULT= " << result);
		return false;
	}
	
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	D3D11_BUFFER_DESC lightBufferDesc;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer
	result = device->CreateBuffer(&lightBufferDesc,nullptr,&_pLightBuffer);
	if(FAILED(result))
	{
		DBG_OUTPT("GNormalMapShader::initializeShader - error creating light buffer HRESULT= " << result);
		return false;
	}
	return true;
}
bool GNormalMapShader::setShaderParameters(ID3D11DeviceContext* deviceContext,
											const GMatrix4x4& world, const GMatrix4x4& view, const GMatrix4x4& projection,
											ID3D11ShaderResourceView** textureArray,
											const GVector3& lightDirection, const GColor& diffuseColor, const GColor& ambientColor)
{
	// Transpose the matrices to prepare them for the shader.
	XMMATRIX worldM = XMMatrixTranspose( world.toXMMatrix());
	XMMATRIX viewM = XMMatrixTranspose( view.toXMMatrix() );
	XMMATRIX projectionM = XMMatrixTranspose( projection.toXMMatrix() );

	// Lock the constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = deviceContext->Map(_pMatrixBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource);
	if(FAILED(result))
	{
		DBG_OUTPT("GNormalMapShader::setShaderParameters - error mapping matrix subresource HRESULT= " << result);
		return false;
	}

	MatrixBufferType* dataPtr= (MatrixBufferType*)mappedResource.pData;
	// Copy the matrices into the constant buffer.
	XMStoreFloat4x4( &dataPtr->world, worldM);
	XMStoreFloat4x4( &dataPtr->view, viewM);
	XMStoreFloat4x4( &dataPtr->projection, projectionM);
	deviceContext->Unmap(_pMatrixBuffer, 0);					// Unlock the constant buffer
	unsigned int bufferNumber = 0;								//Set the position of the constant buffer in the vertex shader
	
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_pMatrixBuffer);	//set the constant buffer in the vertex shader with the updated values.
	
	// Set shader texture array resource in the pixel shader.
	deviceContext->PSSetShaderResources(0,2,textureArray);

	//lock the light constant buffer
	result = deviceContext->Map(_pLightBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource);
	if(FAILED(result))
	{
		DBG_OUTPT("GNormalMapShader::setShaderParameters - error mapping light subresource HRESULT= " << result);
		return false;
	}
	LightBufferType* dataPtr2 = (LightBufferType*)mappedResource.pData;
	//copy the lightning variables into the constant buffer
	dataPtr2->ambientColor.x = ambientColor.red;
	dataPtr2->ambientColor.y = ambientColor.green;
	dataPtr2->ambientColor.z = ambientColor.blue;
	dataPtr2->ambientColor.w = ambientColor.alpha;
	
	dataPtr2->diffuseColor.x = diffuseColor.red;
	dataPtr2->diffuseColor.y = diffuseColor.green;
	dataPtr2->diffuseColor.z = diffuseColor.blue;
	dataPtr2->diffuseColor.w = diffuseColor.alpha;

	dataPtr2->lightDirection.x = lightDirection.x();
	dataPtr2->lightDirection.y = lightDirection.y();
	dataPtr2->lightDirection.z = lightDirection.z();

	dataPtr2->padding = 0.0f;
	deviceContext->Unmap(_pLightBuffer,0);						//unlock the constant buffer
	bufferNumber = 0;
	//set the light buffer in the pixel shader
	deviceContext->PSSetConstantBuffers(bufferNumber,1,&_pLightBuffer);	

	return true;
}

void GNormalMapShader::renderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(_pLayout);
	// Set the vertex and pixel shaders
	deviceContext->VSSetShader(_pVertexShader, nullptr, 0);
	deviceContext->PSSetShader(_pPixelShader, nullptr, 0);
	// Set the sampler stat in the pixel shader
	deviceContext->PSSetSamplers(0,1,&_pSampleState);

	// Render 
	deviceContext->DrawIndexed(indexCount, 0, 0);
	return;
}


#include "GColorShader.h"
#include <GEngineCore.h>


GColorShader::GColorShader(void)	:   GAbstractShader()
{
}

bool GColorShader::initialize(ID3D11Device* device, HWND hWnd)
{
	bool result = initializeShader(device,hWnd,"shaders/color.vs","shaders/color.ps");
	if(!result)
	{
		return false;
	}
	return true;
}

bool GColorShader::render(ID3D11DeviceContext* deviceContext, int indexCount,
			const GMatrix4x4& world, const GMatrix4x4& view, const GMatrix4x4& projection)
{
	bool result = setShaderParameters(deviceContext,world,view,projection);
	if(!result)
	{
		return false;
	}
	renderShader(deviceContext,indexCount);
	return true;
}
												// protected
void GColorShader::unloadContent()
{
}

bool GColorShader::initializeShader(ID3D11Device* device, HWND hWnd, char* vsFilePath, char* psFilePath)
{
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	
	//compile vertex shader
	HRESULT result = compileD3DShader(vsFilePath,"ColorVertexShader","vs_4_0",&vertexShaderBuffer,hWnd);

	if( FAILED (result) )
	{
		return false;
	}

	//compile pixel shader
	result = compileD3DShader(psFilePath,"ColorPixelShader","ps_4_0",&pixelShaderBuffer,hWnd);
	if(FAILED(result) )
	{
		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
										nullptr, &_pVertexShader);
	if(FAILED(result))
	{
		DBG_OUTPT("GColorShader::initializeShader - error creating vertex shader HRESULT= " << result);
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(),
										NULL, &_pPixelShader);
	if(FAILED(result))
	{
		DBG_OUTPT("GColorShader::initializeShader - error creating pixel shader HRESULT= " << result);
		return false;
	}

	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType stucture in the GModel class and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
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
		DBG_OUTPT("GColorShader::initializeShader - error creating input layout HRESULT= " << result);
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
		DBG_OUTPT("GColorShader::initializeShader - error creating matrix buffer HRESULT= " << result);
		return false;
	}

	return true;
}



bool GColorShader::setShaderParameters(ID3D11DeviceContext* deviceContext,
							const GMatrix4x4& world, const GMatrix4x4& view, const GMatrix4x4& projection)
{
	// Transpose the matrices to prepare them for the shader.
	
	XMMATRIX worldM = XMMatrixTranspose( XMMATRIX (world.const_data() ));
	XMMATRIX viewM = XMMatrixTranspose( XMMATRIX (view.const_data() ) );
	XMMATRIX projectionM = XMMatrixTranspose( XMMATRIX (projection.const_data() ) );

	// Lock the constant buffer so it can be written to.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = deviceContext->Map(_pMatrixBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	MatrixBufferType* dataPtr= (MatrixBufferType*)mappedResource.pData;
	// Copy the matrices into the constant buffer.
	
	XMStoreFloat4x4( &dataPtr->world, worldM);
	XMStoreFloat4x4( &dataPtr->view, viewM);
	XMStoreFloat4x4( &dataPtr->projection, projectionM);

	// Unlock the constant buffer
	deviceContext->Unmap(_pMatrixBuffer, 0);

	//Now set the updated matrix buffer in the HLSL vertex shader
	unsigned int bufferNumber = 0;

	//set the constant buffer in the vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_pMatrixBuffer);

	return true;
}

void GColorShader::renderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(_pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(_pVertexShader, NULL, 0);
	deviceContext->PSSetShader(_pPixelShader, NULL, 0);

	// Render 
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}



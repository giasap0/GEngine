#include "GRenderWindow.h"
#include <GEngineCore.h>

GRenderWindow::GRenderWindow(void) : _pVertexBuffer(nullptr), _pIndexBuffer(nullptr)
{
}

void GRenderWindow::shutDown()
{
	shutDownBuffers();
}



bool GRenderWindow::initialize(ID3D11Device* device, int screenWidth, int screenHeight,	int spriteWidth, int spriteHeight)
{
	_screenWidth= screenWidth;			_screenHeight=screenHeight;
	_width=spriteWidth;					_height= spriteHeight;
	_prevPosX= _prevPosY= -1;

	bool result = initializeBuffers(device);
	if(!result)
	{
		DBG_OUTPT("GRenderWindow - cannot initialize buffers");
		return false;
	}

	return true;
}

bool GRenderWindow::render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	// Re-build the dynamic vertex buffer for rendering to possibly a different location on the screen.
	bool result = updateBuffers(deviceContext, positionX, positionY);
	if(!result)
	{
		DBG_OUTPT("GRenderWindow::render - cannot update buffers");
		return false;
	}

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	renderBuffers(deviceContext);

	return true;
}

												// private
bool GRenderWindow::initializeBuffers(ID3D11Device* device)
{
	_vertexCount = _indexCount = 6;
	VertexData* vertices = new VertexData[_vertexCount];
	if(vertices == nullptr)					{return false;}

	Int32* indices = new Int32[_indexCount];
	if(indices == nullptr)					{return false;}

	memset(vertices,0,(sizeof(VertexData)*_vertexCount));
	for(int i =0; i<_indexCount; ++i)		{indices[i]=i;}

	// Set up the description of the dynamic vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexData) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	
	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_pVertexBuffer);
	if(FAILED(result))
	{
		DBG_OUTPT("GRenderWindow::initializeBuffers - failed creating vertex buffer HRESULT = " << result);
		return false;
	}

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(Int32) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_pIndexBuffer);
	if(FAILED(result))
	{
		DBG_OUTPT("GRenderWindow::initializeBuffers - failed creating index buffer HRESULT = " << result);
		return false;
	}

	delete [] vertices;
	delete [] indices;
	vertices = nullptr;
	indices = nullptr;

	return true;

}

void GRenderWindow::shutDownBuffers()
{
	if(_pIndexBuffer!= nullptr)
	{
		_pIndexBuffer->Release();
		_pIndexBuffer = nullptr;
	}
	if(_pVertexBuffer!=nullptr)
	{
		_pVertexBuffer->Release();
		_pVertexBuffer = nullptr;
	}
}

//called each frame to update the contents of the dynamic vertex buffer to re-position the 2D bitmap image on the screen if need be.
bool GRenderWindow::updateBuffers(ID3D11DeviceContext* deviceContext, Int32 positionX, Int32 positionY)
{
	//if position not changed don't update vertex buffer
	if( (positionX==_prevPosX) && (positionY== _prevPosY) )
	{
		return true;
	}
	_prevPosX = positionX;
	_prevPosY = positionY;

	// Calculate the screen coordinates of the bitmap.
	float left = (float)((_screenWidth / 2) * -1) + (float)positionX;
	float right = left + (float)_width;
	float top = (float)(_screenHeight / 2) - (float)positionY;
	float bottom = top - (float)_height;

	VertexData* vertices = new VertexData[_vertexCount];
	if( vertices == nullptr)				{return false;}
	//first triangle
	vertices[0].position = XMFLOAT3(left,top,.0f);
	vertices[0].texture = XMFLOAT2(0.0f,0.0f);
	vertices[1].position = XMFLOAT3(right,bottom,0.0f);
	vertices[1].texture = XMFLOAT2(1.0f,1.0f);
	vertices[2].position = XMFLOAT3(left,bottom,0.0f);
	vertices[2].texture = XMFLOAT2(0.0f,1.0f);
	//second triangle
	vertices[3].position = XMFLOAT3(left,top,0.0f);
	vertices[3].texture = XMFLOAT2(0.0f,0.0f);
	vertices[4].position = XMFLOAT3(right,top,0.0f);
	vertices[4].texture = XMFLOAT2(1.0f,0.0f);
	vertices[5].position = XMFLOAT3(right,bottom,0.0f);
	vertices[5].texture = XMFLOAT2(1.0f,1.0f);

	//copy the contents of the vertex array into the vertex buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = deviceContext->Map(_pVertexBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&mappedResource);
	if( FAILED(result) )
	{
		DBG_OUTPT("GRenderWindow::updateBuffers - failed locking the vertex buffer HRESULT = "<<result);
		return false;
	}
	VertexData* verticesPtr = (VertexData*)mappedResource.pData;

	//copy data into the vertex buffer
	memcpy(verticesPtr,(void*)vertices,(sizeof(VertexData)*_vertexCount) );
	deviceContext->Unmap(_pVertexBuffer,0);

	delete[] vertices;
	vertices= nullptr;
	return true;
}

void GRenderWindow::renderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride = sizeof(VertexData);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0,1,&_pVertexBuffer,&stride,&offset);
	deviceContext->IASetIndexBuffer(_pIndexBuffer,DXGI_FORMAT_R32_UINT,0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


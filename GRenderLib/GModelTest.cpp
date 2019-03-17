#include "GModelTest.h"


GModelTest::GModelTest() : _pVertexBuffer(nullptr), _pIndexBuffer(nullptr) ,_pModel(nullptr), _pTextureArray(nullptr)
{
}

void GModelTest::shutDown()
{
	releaseTextures();
	shutDownBuffers();
	releaseModel();
}

bool GModelTest::initialize(ID3D11Device* d3dDevice, const char* modelFileName, const char* texture1FileName, const char* texture2FileName)
{
	//load model data
	bool result = loadModel(modelFileName);
	if(!result)		{return false;}

	//initialize the vertex and index buffer that hold the geometry
	result = initializeBuffers(d3dDevice);
	if(!result)		{return false;}

	//load textures
	result = loadTextures(d3dDevice,texture1FileName, texture2FileName);
	if(!result)		{return false;}

	return true;
}

void GModelTest::render(ID3D11DeviceContext* d3dDeviceCont)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	renderBuffers(d3dDeviceCont);
}
bool GModelTest::initializeBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//create a vertex array
	VertexData* vertices = new VertexData[_vertexCount];		//vertexCount init in loadModel
	if(vertices == nullptr)
	{
		DBG_OUTPT("GModelTest - cannot allocate memory for vertices (number of vertices =" << _vertexCount << ")");
		return false;
	}
	//create the index array
	unsigned long* indices = new unsigned long[_indexCount];
	if(indices == nullptr)			{return false;}

	// Load the vertex and index arrays with data.
	for(int i=0; i<_vertexCount; ++i)
	{
		vertices[i].position = _pModel[i].pos;
		vertices[i].texture = _pModel[i].text;
		indices[i]=i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexData) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_pVertexBuffer);
	if(FAILED(result))
	{
		DBG_OUTPT("GModelTest::initializeBuffers failed creating Vertex Buffer");
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_pIndexBuffer);
	if(FAILED(result))
	{
		DBG_OUTPT("GModelTest::initializeBuffers failed creating Index Buffer");
		return false;
	}
	delete [] vertices;
	vertices = nullptr;
	delete [] indices;
	indices = nullptr;

	return true;
}

void GModelTest::shutDownBuffers()
{
	if(_pIndexBuffer != nullptr)
	{
		_pIndexBuffer->Release();
		_pIndexBuffer = nullptr;
	}
	if(_pVertexBuffer != nullptr)
	{
		_pVertexBuffer->Release();
		_pVertexBuffer = nullptr;
	}
}

void GModelTest::renderBuffers(ID3D11DeviceContext* deviceContext)
{
	// Set vertex buffer stride and offset.
	unsigned int stride = sizeof(VertexData); 
	unsigned int offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool GModelTest::loadTextures(ID3D11Device* device, const char* fileName1, const char* fileName2)
{
	_pTextureArray = new GTextureArray();
	if(_pTextureArray==nullptr)			{return false;}

	GStringList fileNames;
	fileNames.append(fileName1);
	fileNames.append(fileName2);

	bool result = _pTextureArray->initialize(device,fileNames);
	if(!result)							{return false;}

	return true;
}
void GModelTest::releaseTextures()
{
	if(_pTextureArray != nullptr)
	{
		_pTextureArray->shutDown();
		delete _pTextureArray;
		_pTextureArray = nullptr;
	}
}

bool GModelTest::loadModel(const char* modelFileName)
{
	return GModelLoader::loadStaticModel(modelFileName,_vertexCount,_indexCount,&_pModel);
}

void GModelTest::releaseModel()
{
	if (_pModel != nullptr)
	{
		delete[] _pModel;
		_pModel = nullptr;
	}
}

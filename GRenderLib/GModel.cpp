#include "GRenderIncludes.h"
#include "GModel.h"


GModel::GModel(void) : _pVertexBuffer(nullptr), _pIndexBuffer(nullptr), _pTexture(nullptr), _pModel(nullptr)
{
}

void GModel::shutDown()
{
	releaseTexture();
	shutDownBuffers();
	releaseModel();
}

bool GModel::initialize(ID3D11Device* d3dDevice, const char* modelFileName, const char* textureFileName)
{	
	//load model data
	bool result = loadModel(modelFileName);
	if(!result)
	{
		return false;
	}

	// Initialize the vertex and index buffer that hold the geometry
	result = initializeBuffers(d3dDevice);
	if(!result)
	{
		return false;
	}

	result = loadTexture(d3dDevice,textureFileName);
	if(!result)
	{
		return false;
	}

	return true;
}

void GModel::render(ID3D11DeviceContext* d3dDeviceCont)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	renderBuffers(d3dDeviceCont);
}

bool GModel::initializeBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//create a vertex array
	VertexData* vertices = new VertexData[_vertexCount];		//vertexCount init in loadModel
	if(vertices == nullptr)
	{
		DBG_OUTPT("GModel::initializeBuffers - cannot allocate memory for vertices (number of vertices =" << _vertexCount << ")");
		return false;
	}
	//create the index array
	unsigned long* indices = new unsigned long[_indexCount];
	if(indices == nullptr)
	{
		return false;
	}

	// Load the vertex and index arrays with data.
	for(int i=0; i<_vertexCount; ++i)
	{
		vertices[i].position = _pModel[i].pos;
		vertices[i].normal = _pModel[i].normal;
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
		DBG_OUTPT("GModel::initializeBuffers - failed creating Vertex Buffer");
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
		DBG_OUTPT("GModel::initializeBuffers - failed creating Index Buffer");
		return false;
	}
	delete [] vertices;
	vertices = nullptr;
	delete [] indices;
	indices = nullptr;

	return true;
}

void GModel::shutDownBuffers()
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
void GModel::renderBuffers(ID3D11DeviceContext* deviceContext)
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

bool GModel::loadTexture(ID3D11Device* device, const char* fileName)
{
	_pTexture = new GTexture();
	if(_pTexture==nullptr)
	{
		return false;	
	}

	bool result = _pTexture->initialize(device,fileName);
	if(!result)
	{
		return false;
	}
	return true;
}

void GModel::releaseTexture()
{
	if(_pTexture!= nullptr)
	{
		_pTexture->shutDown();
		delete _pTexture;
		_pTexture = nullptr;
	}
}

bool GModel::loadModel(const char* modelFileName)
{
	return GModelLoader::loadStaticModel(modelFileName,_vertexCount,_indexCount,&_pModel);
}
	

void GModel::releaseModel()
{
	if(_pModel!= nullptr)
	{
		delete[] _pModel;
		_pModel = nullptr;
	}
}

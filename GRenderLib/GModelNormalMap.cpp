#include "GModelNormalMap.h"
//#include <cstdlib>
//#include <fstream>

GModelNormalMap::GModelNormalMap(void) : _pVertexBuffer(nullptr), _pIndexBuffer(nullptr),
										_pTextures(nullptr), _pModel(nullptr)
{
}

void GModelNormalMap::shutDown()
{
	releaseTextures();
	shutDownBuffers();
	releaseModel();
}

bool GModelNormalMap::initialize(ID3D11Device* d3dDevice, const char* modelFileName, const char* textureFileName, const char* normalMapFileName)
{
	bool result = loadModel(modelFileName);
	if( !result )		{return false;}

	// Initialize the vertex and index buffer that hold the geometry
	result = initializeBuffers(d3dDevice);
	if(!result)			{return false;}

	//load textures
	result = loadTextures(d3dDevice,textureFileName,normalMapFileName);
	if(!result)			{return false;}

	return true;
}
void GModelNormalMap::render(ID3D11DeviceContext* d3dDeviceCont)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	renderBuffers(d3dDeviceCont);
}

bool GModelNormalMap::initializeBuffers(ID3D11Device* device)
{
	//create a vertex array
	VertexData* vertices = new VertexData[_vertexCount];		//vertexCount init in loadModel
	if(vertices == nullptr)
	{
		DBG_OUTPT("GModelNormalMap::initializeBuffers - cannot allocate memory for vertices (number of vertices =" << _vertexCount << ")");
		return false;
	}
	//create the index array
	unsigned long* indices = new unsigned long[_indexCount];
	if(indices == nullptr)	{return false;}

	// Load the vertex and index arrays with data.
	for(int i=0; i<_vertexCount; ++i)
	{
		vertices[i].position = _pModel[i].pos;
		vertices[i].texture = _pModel[i].texture;
		vertices[i].normal = _pModel[i].normal;
		vertices[i].tangent = _pModel[i].tangent;
		vertices[i].binormal = _pModel[i].binormal;
		indices[i]=i;
	}

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexData) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
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
		DBG_OUTPT("GModelNormalMap::initializeBuffers - failed creating Vertex Buffer");
		return false;
	}

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
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
		DBG_OUTPT("GModelNormalMap::initializeBuffers - failed creating Index Buffer");
		return false;
	}
	delete [] vertices;
	vertices = nullptr;
	delete [] indices;
	indices = nullptr;

	return true;
}

void GModelNormalMap::shutDownBuffers()
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
void GModelNormalMap::renderBuffers(ID3D11DeviceContext* deviceContext)
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

bool GModelNormalMap::loadTextures(ID3D11Device* device, const char* textFileName, const char* normalMapFileName)
{
	_pTextures = new GTextureArray();
	if(_pTextures==nullptr)	{return false;}

	GStringList fileNames;
	fileNames.append(textFileName);
	fileNames.append(normalMapFileName);

	bool result = _pTextures->initialize(device,fileNames);
	if(!result)				{return false;}

	return true;
}

void GModelNormalMap::releaseTextures()
{
	if(_pTextures!= nullptr)
	{
		_pTextures->shutDown();
		delete _pTextures;
		_pTextures = nullptr;
	}
}

bool GModelNormalMap::loadModel(const char* modelFileName)
{
	bool res = GModelLoader::loadNormalMapModelFromStaticModel(modelFileName, _vertexCount, _indexCount, &_pModel);
	/*
	std::ofstream fout;
	fout.open("model normal map.txt");
	fout << "GModelNormalMap::loadModel - model loaded = " << res << "\n";
	fout << "model file name = " << modelFileName << "\n";
	fout << "Vertex count = " << _vertexCount << "\n";
	fout << "Index count = "<< _indexCount << "\n";
	fout << "MODEL vertices: " << "\n";
	for(int i=0; i<_vertexCount; i++)
	{
		fout << "vertice "<<i<<") ("<< _pModel[i].pos.x << ", " << _pModel[i].pos.y<< ", " << _pModel[i].pos.z<< ")" << "\n";
		fout << "texture "<<i<<") ("<< _pModel[i].texture.x << ", " << _pModel[i].texture.y<< ")" << "\n";
		fout << "normal "<<i<<") ("<< _pModel[i].normal.x << ", " << _pModel[i].normal.y<< ", " << _pModel[i].normal.z<< ")" << "\n";
		fout << "binormal "<<i<<") ("<< _pModel[i].binormal.x << ", " << _pModel[i].binormal.y<< ", " << _pModel[i].binormal.z<< ")" << "\n";
		fout << "tg "<<i<<") ("<< _pModel[i].tangent.x << ", " << _pModel[i].tangent.y<< ", " << _pModel[i].tangent.z<< ")" << "\n";
	}
	fout.close();
	*/
	return res;
}
	

void GModelNormalMap::releaseModel()
{
	if(_pModel!= nullptr)
	{
		delete[] _pModel;
		_pModel = nullptr;
	}
}

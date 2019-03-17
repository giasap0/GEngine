#include "GModel.h"
#include "GModelLoader.h"


GModel::GModel(void) : _pVertexBuffer(nullptr), _pIndexBuffer(nullptr), _pTexture(nullptr), _pModel(nullptr)
{
	_width = _height = _length = _radius = 0;
	_vertexCount = _indexCount= 0;
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
		return false;

	//initialize the vertex and index buffer that hold the geometry
	result = initializeBuffers(d3dDevice);
	if(!result)
		return false;

	result = loadTexture(d3dDevice,textureFileName);
	if(!result)
		return false;

	return true;
}

void GModel::render(ID3D11DeviceContext* d3dDeviceCont)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	renderBuffers(d3dDeviceCont);
}


bool GModel::initializeBuffers(ID3D11Device* device)
{
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
		vertices[i].position = _pModel[i].position;
		vertices[i].normal =   _pModel[i].normal;
		vertices[i].texture =  _pModel[i].texture;
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
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_pVertexBuffer);
	if(FAILED(result))
	{
		DBG_OUTPT("GModel::initializeBuffers - failed creating Vertex Buffer");
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

bool GModel::loadTexture(ID3D11Device* device, const char* textFileName)
{
	_pTexture = new GTexture();
	if(_pTexture==nullptr)	{return false;}

	bool result = _pTexture->initialize(device,textFileName);
	if(!result)				{return false;}

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

void GModel::releaseModel()
{
	if (_pModel!= nullptr)
	{
		delete[] _pModel;
		_pModel = nullptr;
	}
}


bool GModel::loadModel(const char* modelFileName)
{
	releaseModel();

	GModelLoader modelLoader;
	bool result = modelLoader.loadModel(modelFileName);
	if(!result)
	{
		DBG_OUTPT("GModel::loadModel - failed loading model file "<< modelFileName);
		return false;
	}
	
	_vertexCount =  modelLoader.getTotalVerts();
	if(_vertexCount == 0)
	{
		DBG_OUTPT("GModel::loadModel - in model file "<< modelFileName << " vertex count == 0");
		return false;
	}

	_indexCount= _vertexCount;
	_pModel = new VertexData[ _vertexCount ];
	if( _pModel == nullptr)
	{
		DBG_OUTPT("GModel::loadModel - model file "<< modelFileName <<" cannot allocate "<< _vertexCount << " vertex");
		return false;
	}


	bool haveNormals = modelLoader.haveNormal();
	bool haveTextureCoords = modelLoader.haveTexCoord();
	int vIndex = 0, tIndex=0;

	for(int v=0; v<_vertexCount;v++)
	{
		_pModel[v].position.x= modelLoader.getVertices()->at( vIndex + 0);
		_pModel[v].position.y= modelLoader.getVertices()->at( vIndex + 1);
		_pModel[v].position.z= modelLoader.getVertices()->at( vIndex + 2);

		if (haveNormals)
		{
			_pModel[v].normal.x = modelLoader.getNormals()->at( vIndex +0);
			_pModel[v].normal.y = modelLoader.getNormals()->at( vIndex +1);
			_pModel[v].normal.z = modelLoader.getNormals()->at( vIndex +2);
		}
		else
		{
			_pModel[v].normal = DirectX::XMFLOAT3(.0f,.0f,.0f);
		}

		if(haveTextureCoords)
		{
			_pModel[v].texture.x = modelLoader.getTexCoords()->at( tIndex +0);
			_pModel[v].texture.y = modelLoader.getTexCoords()->at( tIndex +1);
		}
		else
		{
			_pModel[v].texture = DirectX::XMFLOAT2(.0f,.0f);
		}

		vIndex+=3;
		tIndex+=2;
	}//EO for(vertices)

	calculateBounds(_center,_width,_height,_length,_radius);
	return true;
}


void GModel::normalize(GMath::Real scaleTo, bool center)
{
	using GMath::Real;

	Real width = .0f, height= .0f, length= .0f, radius= .0f;
	GVector3 centerPos;

   calculateBounds(centerPos, width, height, length, radius);

    Real scalingFactor = scaleTo / radius;
	GVector3 offset;

    if (center)
    {
		offset = -centerPos;
    }
    else
    {
		offset.setValues(0,0,0);
    }

    scale(scalingFactor, offset);
    calculateBounds(_center, _width, _height, _length, _radius);
}

void GModel::scale(GMath::Real scaleFactor, const GVector3& offset)
{
	for (int i = 0; i < _vertexCount; ++i)
    {
		_pModel[i].position.x += offset[0];
        _pModel[i].position.y += offset[1];
        _pModel[i].position.z += offset[2];

		if( ! GMath::areEqual(scaleFactor,1.0f) )
		{
			_pModel[i].position.x *= scaleFactor;
			_pModel[i].position.y *= scaleFactor;
			_pModel[i].position.z *= scaleFactor;
		}
    }//EO for( vertex )
}

void GModel::calculateBounds(_Out_ GVector3& center,
						_Out_ GMath::Real& width,_Out_ GMath::Real& height,
						_Out_ GMath::Real& length,_Out_ GMath::Real& radius) const
{
	using GMath::Real;
	
	if(_vertexCount<1)
	{
		DBG_OUTPT("GModel::calculateBounds - request calculate bounds on an empty model");
		return;
	}

	Real xMin, yMin, zMin, xMax, yMax, zMax;
	xMin = xMax = _pModel[0].position.x;
	yMin = yMax = _pModel[0].position.y;
	zMin = zMax = _pModel[0].position.z;

    Real x = 0.0f, y = 0.0f, z = 0.0f;
	uint numVerts = _vertexCount;

    for (uint i = 0; i < numVerts; ++i)
    {
		x = _pModel[i].position.x;
		y = _pModel[i].position.y;
		z = _pModel[i].position.z;

        if (x < xMin)
            xMin = x;

        if (x > xMax)
            xMax = x;

        if (y < yMin)
            yMin = y;

        if (y > yMax)
            yMax = y;

        if (z < zMin)
            zMin = z;

        if (z > zMax)
            zMax = z;
    }

    center[0] = (xMin + xMax) / 2.0f;
    center[1] = (yMin + yMax) / 2.0f;
    center[2] = (zMin + zMax) / 2.0f;

    width = xMax - xMin;
    height = yMax - yMin;
    length = zMax - zMin;

	radius = GMath::maximum( GMath::maximum(width, height) , length)/2.0f;
}

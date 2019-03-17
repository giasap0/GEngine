//------------------------------------------------------------------------------------
//Parts of this code are inspired from from a dhpoware's demo.
//http://www.dhpoware.com/demos/glObjViewer.html
//-----------------------------------------------------------------------------------

#include "GModel_obj.h"
//#include <GVector2.h>

GModel_obj::GModel_obj(void) : _pVertexBuffer(nullptr), _pIndexBuffer(nullptr), _pMeshParts(nullptr)
{
	_hasPositions = _hasNormals = _hasTextureCoords = _hasTangents = false;
	_vertexCount = _indexCount = _numberOfTextureCoords = _numberOfNormals = _numberOfTriangles = _numberOfMaterials = _numberOfMeshes = 0;
	_width = _height = _length = _radius = 0.0f;
}

void GModel_obj::shutDown()
{
	releaseMeshes();
	shutDownBuffers();
	releaseModel();

	_directoryPath.clear();

	_hasPositions = _hasNormals = _hasTextureCoords = _hasTangents = false;
	_vertexCount = _indexCount = _numberOfTextureCoords = _numberOfNormals = _numberOfTriangles = _numberOfMaterials = _numberOfMeshes = 0;
	_width = _height = _length = _radius = 0.0f;
	_center.setValues(0,0,0);
}

bool GModel_obj::initialize(ID3D11Device* d3dDevice, const char* modelFileName)
{
	//_directoryPath = modelFileName;
	bool result = loadModel(modelFileName);
	if( !result )
	{
		DBG_OUTPT("GModel_obj::initialize - failed loading model from file "<< modelFileName);
		return false;
	}

	// Initialize the vertex and index buffer that hold the geometry
	result = initializeBuffers(d3dDevice);
	if( !result )
	{
		DBG_OUTPT("GModel_obj::initialize - failed initializing buffers");
		return false;
	}

		//load textures
	result = loadMeshes(d3dDevice);
	if( !result )
	{
		DBG_OUTPT("GModel_obj::initialize - failed loading meshes");
		return false;
	}

	return true;
}

void GModel_obj::render(ID3D11DeviceContext* d3dDeviceCont)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	renderBuffers(d3dDeviceCont);
}

bool GModel_obj::initializeBuffers(ID3D11Device* device)
{
	using namespace ModelsStructs;

	if( _pVertexBuffer != nullptr)
	{
		DBG_OUTPT("GModel_obj::initializeBuffers - vertex buffer is allocated. Deleting all data");
		_pVertexBuffer->Release();
		_pVertexBuffer = nullptr;
	}
	if(_pIndexBuffer != nullptr)
	{
		DBG_OUTPT("GModel_obj::initializeBuffers - index buffer is allocated. Deleting all data");
		_pIndexBuffer->Release();
		_pIndexBuffer = nullptr;
	}

	ObjModel_Vertex* vertices = new ObjModel_Vertex[_vertexCount];
	if(vertices == nullptr)
	{
		DBG_OUTPT("GModel_obj::initializeBuffers - cannot allocate memory for vertices (number of vertices =" << _vertexCount << ")");
		return false;
	}

	// Load the vertex arrays with data.
	for(uint i=0; i<_vertexCount; ++i)
	{
		vertices[i].pos = _model[i].pos;
		vertices[i].texture = _model[i].texture;
		vertices[i].normal = _model[i].normal;
		vertices[i].tangent = _model[i].tangent;
		vertices[i].bitangent = _model[i].bitangent;
	}

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(ObjModel_Vertex) * _vertexCount;
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
		DBG_OUTPT("GModel_obj::initializeBuffers - failed creating Vertex Buffer");
		return false;
	}

	// Set up the description of the static index buffer.
	// Load the index arrays with data.
	_indexCount = _indexBuffer.size();
	//create the index array
	Int32* indices = new Int32[_indexCount];
	if(indices == nullptr)	{return false;}

	for(uint i=0; i< _indexCount; ++i)
	{
		indices[i] = _indexBuffer[i];
	}

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
		DBG_OUTPT("GModel_obj::initializeBuffers - failed creating Index Buffer");
		return false;
	}

	delete [] vertices;
	vertices = nullptr;
	delete [] indices;
	indices = nullptr;

	return true;
}

bool GModel_obj::loadMeshes(ID3D11Device* device)
{
	_pMeshParts = new GMeshesArray();
	if(_pMeshParts ==nullptr)	{return false;}

	_numberOfMeshes = _meshes.size();
	bool result = _pMeshParts->initialize(device,_meshes);
	if(!result)				{return false;}

	return true;
}

void GModel_obj::renderBuffers( ID3D11DeviceContext* deviceContext)
{
	//set vertex buffer stride and offset
	unsigned int stride = sizeof(ModelsStructs::ObjModel_Vertex);
	unsigned int offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void GModel_obj::normalize(Real scaleTo, bool center)
{
	Real width, height, length, radius;
	GVector3 centerPos;
	width = height = length = radius = 0.0f;

    bounds(centerPos, width, height, length, radius);

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
    bounds(_center, _width, _height, _length, _radius);
}

void GModel_obj::scale(Real scaleFactor, const GVector3& offset)
{
	for (uint i = 0; i < _model.size(); ++i)
    {
        _model[i].pos.x += offset[0];
        _model[i].pos.y += offset[1];
        _model[i].pos.z += offset[2];

        _model[i].pos.x *= scaleFactor;
        _model[i].pos.y *= scaleFactor;
        _model[i].pos.z *= scaleFactor;
    }
}

void GModel_obj::reverseWinding()
{
	Int32 swap = 0;

    // Reverse face winding.
    for (uint i = 0; i < _indexBuffer.size(); i += 3)
    {
        swap = _indexBuffer[i + 1];
        _indexBuffer[i + 1] = _indexBuffer[i + 2];
        _indexBuffer[i + 2] = swap;
    }


    // Invert normals and tangents.
	for (uint i = 0; i < _model.size(); ++i)
    {
        _model[i].normal.x = -_model[i].normal.x;
        _model[i].normal.y = -_model[i].normal.y;
		_model[i].normal.z = -_model[i].normal.z;

        _model[i].tangent.x = -_model[i].tangent.x;
        _model[i].tangent.y = -_model[i].tangent.y;
        _model[i].tangent.z = -_model[i].tangent.z;
    }
}
bool GModel_obj::loadModel(const char* modelFileName)
{
	// Import the OBJ file.
	bool res = GModelLoader::loadModelOBJ(modelFileName,_directoryPath,
										_hasPositions,_hasTextureCoords,_hasNormals,_hasTangents,
										_model,_vertexCoords,_textureCoords,_normals,_indexBuffer,_attributeBuffer,_materials,
										_meshes, _materialCache,_vertexCache);
	if(!res)
	{
		DBG_OUTPT("GModel_obj::loadModel - cannot load model from file "<< modelFileName);
		return false;
	}

	_numberOfTriangles = static_cast<uint>(_indexBuffer.size()/3.0f);
	_numberOfNormals = static_cast<uint>(_normals.size()/3.0f);
	_numberOfTextureCoords = static_cast<uint>(_textureCoords.size()/2.0f);
	_vertexCount = static_cast<uint>(_vertexCoords.size()/3.0f);
	_indexCount = _indexBuffer.size();

	//riempire materialCache da materials

	// Perform post import tasks.
    buildMeshes();
    bounds(_center, _width, _height, _length, _radius);

    // Build vertex normals if required.
     if (!hasNormals())
		generateNormals();

    // Build tangents is required.

    for (uint i = 0; i < _numberOfMaterials; ++i)
    {
		if (!_materials[i].normalMap_fileName.isEmpty())
        {
            generateTangents();
            break;
        }
    }

	DBG_OUTPT("GModel_obj - model "<< modelFileName <<" succesfully loaded. Number of meshes = "<< _meshes.size() <<" , number of materials = "<< _numberOfMaterials);

    return true;
}

bool MeshCompFunc(const ModelsStructs::Mesh& lhs, const ModelsStructs::Mesh& rhs)
{
        return lhs.pMaterial->alpha > rhs.pMaterial->alpha;
}

void GModel_obj::buildMeshes()
{
	using namespace ModelsStructs;

	// Group the model's triangles based on material type.
    Int32 materialId = -1;

    // Count the number of meshes.
    Int32 numMeshes = 0;
    for (uint i = 0; i < _attributeBuffer.size(); ++i)
    {
        if (_attributeBuffer[i] != materialId)
        {
            materialId = _attributeBuffer[i];
            ++numMeshes;
        }
    }

    // Allocate memory for the meshes and reset counters.
    _numberOfMeshes = numMeshes;
    _meshes.resize(_numberOfMeshes);
    numMeshes = 0;
    materialId = -1;
	Mesh *pMesh = nullptr;

    // Build the meshes. One mesh for each unique material.
    for (uint i = 0; i < _attributeBuffer.size(); ++i)
    {
        if (_attributeBuffer[i] != materialId)
        {
            materialId = _attributeBuffer[i];
            pMesh = &_meshes[numMeshes++];            
            pMesh->pMaterial = &_materials[materialId];
            pMesh->startIndex = i * 3;
            ++pMesh->triangleCount;
        }
        else
        {
            ++pMesh->triangleCount;
        }
    }

    // Sort the meshes based on its material alpha. Fully opaque meshes towards the front and fully transparent towards the back.
	_meshes.sort(MeshCompFunc);
}

void GModel_obj::bounds(_Out_ GVector3& center,_Out_ Real& width,_Out_ Real& height,_Out_ Real& length,_Out_ Real& radius) const
{
	Real xMin, yMin, zMin, xMax, yMax, zMax;

	xMin = yMin = zMin = GMath::Constants::realMin;
	xMax = yMax = zMax = GMath::Constants::realMax;

    Real x = 0.0f;
    Real y = 0.0f;
    Real z = 0.0f;

	uint numVerts =  _model.size();

    for (uint i = 0; i < numVerts; ++i)
    {
		x = _model[i].pos.x;
		y = _model[i].pos.y;
		z = _model[i].pos.z;

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

    radius = max(max(width, height), length);
}

void GModel_obj::generateNormals()
{
	using namespace ModelsStructs;
	const Int32 *pTriangle = nullptr;
	ObjModel_Vertex* pVertex0 = nullptr;
	ObjModel_Vertex *pVertex1 = nullptr;
	ObjModel_Vertex *pVertex2 = nullptr;
	GVector3 edge1, edge2, normal;
    Real length = 0.0f;
    Int32 totalVertices = getNumberOfVertices();
    Int32 totalTriangles = getNumberOfTriangles();

    // Initialize all the vertex normals.
    for (int i = 0; i < totalVertices; ++i)
    {
		pVertex0 = &_model[i];
        pVertex0->normal.x = 0.0f;
        pVertex0->normal.y = 0.0f;
        pVertex0->normal.z = 0.0f;
    }

    // Calculate the vertex normals.
    for (int i = 0; i < totalTriangles; ++i)
    {
        pTriangle = &_indexBuffer[i * 3];

        pVertex0 = &_model[ pTriangle[0] ];
        pVertex1 = &_model[ pTriangle[1] ];
        pVertex2 = &_model[ pTriangle[2] ];

        // Calculate triangle face normal.
        edge1[0] = pVertex1->pos.x - pVertex0->pos.x;
        edge1[1] = pVertex1->pos.y - pVertex0->pos.y;
        edge1[2] = pVertex1->pos.z - pVertex0->pos.z;

        edge2[0] = pVertex2->pos.x - pVertex0->pos.x;
        edge2[1] = pVertex2->pos.y - pVertex0->pos.y;
        edge2[2] = pVertex2->pos.z - pVertex0->pos.z;

		normal = edge1^edge2;

        // Accumulate the normals.
        pVertex0->normal.x += normal.x();
        pVertex0->normal.y += normal.y();
        pVertex0->normal.z += normal.z();

        pVertex1->normal.x += normal.x();
        pVertex1->normal.y += normal.y();
        pVertex1->normal.z += normal.z();

        pVertex2->normal.x += normal.x();
        pVertex2->normal.y += normal.y();
        pVertex2->normal.z += normal.z();
    }

    // Normalize the vertex normals.
    for (int i = 0; i < totalVertices; ++i)
    {
        pVertex0 = &_model[i];

        length = 1.0f / sqrtf(pVertex0->normal.x * pVertex0->normal.x +
								pVertex0->normal.y * pVertex0->normal.y +
								 pVertex0->normal.z * pVertex0->normal.z);

        pVertex0->normal.x *= length;
        pVertex0->normal.y *= length;
        pVertex0->normal.z *= length;
    }

    _hasNormals = true;
}

void GModel_obj::generateTangents()
{
	ModelsStructs::ObjModel_Vertex* pVertex0 = nullptr;   
    uint32 totalVertices = getNumberOfVertices();
    uint32 totalTriangles = getNumberOfTriangles();

    // Initialize all the vertex tangents and bitangents to 0.
    for (uint i = 0; i < totalVertices; ++i)
    {
        pVertex0 = &_model[i];
		pVertex0->tangent.x = pVertex0->tangent.y = pVertex0->tangent.z = pVertex0->tangent.w = 0.0f;
        pVertex0->bitangent.x = pVertex0->bitangent.y = pVertex0->bitangent.z = 0.0f;
    }

    // Calculate the vertex tangents and bitangents.
	const Int32* pTriangle = nullptr;
	ModelsStructs::ObjModel_Vertex* pVertex1 = nullptr;
	ModelsStructs::ObjModel_Vertex* pVertex2 = nullptr;
	GVector3 edge1, edge2, tangent, bitangent;
	GMath::GVector2 texEdge1, texEdge2;
	Real det = 0.0f;

    for (uint i = 0; i < totalTriangles; ++i)
    {
        pTriangle = &_indexBuffer[i * 3];

        pVertex0 = &_model[ pTriangle[0] ];
        pVertex1 = &_model[ pTriangle[1] ];
        pVertex2 = &_model[ pTriangle[2] ];

        // Calculate the triangle face tangent and bitangent.

        edge1[0] = pVertex1->pos.x - pVertex0->pos.x;
        edge1[1] = pVertex1->pos.y - pVertex0->pos.y;
		edge1[2] = pVertex1->pos.z - pVertex0->pos.z;

        edge2[0] = pVertex2->pos.x - pVertex0->pos.x;
        edge2[1] = pVertex2->pos.y - pVertex0->pos.y;
        edge2[2] = pVertex2->pos.z - pVertex0->pos.z;

        texEdge1[0] = pVertex1->texture.x - pVertex0->texture.x;
        texEdge1[1] = pVertex1->texture.y - pVertex0->texture.y;

        texEdge2[0] = pVertex2->texture.x - pVertex0->texture.x;
        texEdge2[1] = pVertex2->texture.y - pVertex0->texture.y;

        det = texEdge1[0] * texEdge2[1] - texEdge2[0] * texEdge1[1];

        if (fabs(det) < 1e-6f)
        {
			tangent.setValues(1,0,0);
			bitangent.setValues(0,1,0);
        }
        else
        {
            det = 1.0f / det;

            tangent[0] = (texEdge2[1] * edge1[0] - texEdge1[1] * edge2[0]) * det;
            tangent[1] = (texEdge2[1] * edge1[1] - texEdge1[1] * edge2[1]) * det;
            tangent[2] = (texEdge2[1] * edge1[2] - texEdge1[1] * edge2[2]) * det;

            bitangent[0] = (-texEdge2[0] * edge1[0] + texEdge1[0] * edge2[0]) * det;
            bitangent[1] = (-texEdge2[0] * edge1[1] + texEdge1[0] * edge2[1]) * det;
            bitangent[2] = (-texEdge2[0] * edge1[2] + texEdge1[0] * edge2[2]) * det;
        }

        // Accumulate the tangents and bitangents.

        pVertex0->tangent.x += tangent[0];
        pVertex0->tangent.y += tangent[1];
        pVertex0->tangent.z += tangent[2];
        pVertex0->bitangent.x += bitangent[0];
        pVertex0->bitangent.y += bitangent[1];
        pVertex0->bitangent.z += bitangent[2];

        pVertex1->tangent.x += tangent[0];
        pVertex1->tangent.y += tangent[1];
        pVertex1->tangent.z += tangent[2];
        pVertex1->bitangent.x += bitangent[0];
        pVertex1->bitangent.y += bitangent[1];
        pVertex1->bitangent.z += bitangent[2];

        pVertex2->tangent.x += tangent[0];
        pVertex2->tangent.y += tangent[1];
        pVertex2->tangent.z += tangent[2];
        pVertex2->bitangent.x += bitangent[0];
        pVertex2->bitangent.y += bitangent[1];
        pVertex2->bitangent.z += bitangent[2];

    }//EO  for (uint i = 0; i < totalTriangles; ++i)

    // Orthogonalize and normalize the vertex tangents.
	Real bDotB = 0.0f;
	Real nDotT = 0.0f;
	Real length = 0.0f;
    for (uint i = 0; i < totalVertices; ++i)
    {
        pVertex0 = &_model[i];

        // Gram-Schmidt orthogonalize tangent with normal.

        nDotT = pVertex0->normal.x * pVertex0->tangent.x +
                pVertex0->normal.y * pVertex0->tangent.y +
                pVertex0->normal.z * pVertex0->tangent.z;

        pVertex0->tangent.x -= pVertex0->normal.x * nDotT;
        pVertex0->tangent.y -= pVertex0->normal.y * nDotT;
        pVertex0->tangent.z -= pVertex0->normal.z * nDotT;

        // Normalize the tangent.

        length = 1.0f / sqrtf(pVertex0->tangent.x * pVertex0->tangent.x +
                              pVertex0->tangent.y * pVertex0->tangent.y +
                              pVertex0->tangent.z * pVertex0->tangent.z);

        pVertex0->tangent.x *= length;
        pVertex0->tangent.y *= length;
        pVertex0->tangent.z *= length;

        // Calculate the handedness of the local tangent space.
        // The bitangent vector is the cross product between the triangle face
        // normal vector and the calculated tangent vector. The resulting
        // bitangent vector should be the same as the bitangent vector
        // calculated from the set of linear equations above. If they point in
        // different directions then we need to invert the cross product
        // calculated bitangent vector. We store this scalar multiplier in the
        // tangent vector's 'w' component so that the correct bitangent vector
        // can be generated in the normal mapping shader's vertex shader.
        //
        // Normal maps have a left handed coordinate system with the origin
        // located at the top left of the normal map texture. The x coordinates
        // run horizontally from left to right. The y coordinates run
        // vertically from top to bottom. The z coordinates run out of the
        // normal map texture towards the viewer. Our handedness calculations
        // must take this fact into account as well so that the normal mapping
        // shader's vertex shader will generate the correct bitangent vectors.
        // Some normal map authoring tools such as Crazybump
        // (http://www.crazybump.com/) includes options to allow you to control
        // the orientation of the normal map normal's y-axis.

        bitangent[0] = (pVertex0->normal.y * pVertex0->tangent.z) - 
                       (pVertex0->normal.z * pVertex0->tangent.y);
        bitangent[1] = (pVertex0->normal.z * pVertex0->tangent.x) -
                       (pVertex0->normal.x * pVertex0->tangent.z);
        bitangent[2] = (pVertex0->normal.x * pVertex0->tangent.y) - 
                       (pVertex0->normal.y * pVertex0->tangent.x);

        bDotB = bitangent[0] * pVertex0->bitangent.x + 
                bitangent[1] * pVertex0->bitangent.y + 
                bitangent[2] * pVertex0->bitangent.z;

		pVertex0->tangent.w = (bDotB < 0.0f) ? 1.0f : -1.0f;

        pVertex0->bitangent.x = bitangent[0];
        pVertex0->bitangent.y = bitangent[1];
        pVertex0->bitangent.z = bitangent[2];

    }	//EO for (int i = 0; i < totalVertices; ++i)

    _hasTangents = true;
}

void GModel_obj::releaseMeshes()
{
	if(_pMeshParts!= nullptr)
	{
		_pMeshParts->shutDown();
		delete _pMeshParts;
		_pMeshParts = nullptr;
	}
}
void GModel_obj::shutDownBuffers()
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
void GModel_obj::releaseModel()
{
	_meshes.clear();
	_materials.clear();
	_model.clear();
	_attributeBuffer.clear();
	_indexBuffer.clear();
	_vertexCoords.clear();
	_textureCoords.clear();
	_normals.clear();

	_materialCache.clear();
	_vertexCache.clear();
}

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
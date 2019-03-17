//------------------------------------------------------------------------------------
//Parts of this code are inspired from from a dhpoware's demo.
//http://www.dhpoware.com/demos/glObjViewer.html
//-----------------------------------------------------------------------------------

#pragma once
#ifndef GMODEL_OBJ_H
#define GMODEL_OBJ_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include "GMeshesArray.h"
#include <GString.h>

//-----------------------------------------------------------------------------
// Class GModel_obj : get a model from a .obj file
//----------------------------------------------------------------------------
class GModel_obj
{
public:
	GModel_obj(void);
	virtual ~GModel_obj(void)			{}

	//handle initializing and shutdown of the model's vertex and index buffers

	void shutDown();
	bool initialize(ID3D11Device* d3dDevice, const char* modelFileName);

	// Puts the model geometry on the video card to prepare it for drawing by the color shader.
	void render(ID3D11DeviceContext* d3dDeviceCont);
	void normalize(Real scaleTo = 1.0f, bool center = true);
	// Reverse face winding.
    void reverseWinding();

	// Getter methods.

	//informations about all materials
	inline GMeshesArray*	getMeshesArray()			{return _pMeshParts;}

	inline GVector3	getCenter() const	{return _center;}
	inline Real		getWidth() const	{return _width;}
	inline Real		getHeight() const	{return _height;}
	inline Real		getLength() const	{return _length;}
	inline Real		getRadius() const	{return _radius;}

    inline Int32 getIndexSize() const	{ return static_cast<Int32>(sizeof(Int32)); }


	inline const ModelsStructs::Material& getMaterial(uint i) const		{return _materials[i];}
	inline const ModelsStructs::Mesh& getMesh(uint i) const				{return _meshes[i];}

	inline uint32 getNumberOfIndices() const				{return _numberOfTriangles*3;}
	inline uint32 getNumberOfMaterials() const				{return _numberOfMaterials;}
	inline uint32 getNumberOfMeshes() const					{return _numberOfMeshes;}
	inline uint32 getNumberOfTriangles() const				{return _numberOfTriangles;}
	inline uint32 getNumberOfVertices() const				{return _model.size();}

	inline const GString& getPath() const					{return _directoryPath;}

	inline size_t getVertexSize() const						{return sizeof(ModelsStructs::ObjModel_Vertex);}

	inline bool hasNormals() const							{return _hasNormals;}
	inline bool hasPositions() const						{return _hasPositions;}
	inline bool hasTangents() const							{return _hasTangents;}
	inline bool hasTextureCoords() const					{return _hasTextureCoords;}

private:
	bool initializeBuffers(ID3D11Device* d3dDevice);
	void shutDownBuffers();
	void renderBuffers(ID3D11DeviceContext* d3dDeviceCont);
	
	bool loadMeshes(ID3D11Device* device);
	void releaseMeshes();
	bool loadModel(const char* modelFileName);
	void releaseModel();

	void bounds(_Out_ GVector3& center,_Out_ Real& width,_Out_ Real& height,_Out_ Real& length,_Out_ Real& radius) const;
    void buildMeshes();
    void generateNormals();
    void generateTangents();
	void scale(Real scaleFactor, const GVector3& offset = GVector3() );

    bool _hasPositions;
    bool _hasTextureCoords;
    bool _hasNormals;
    bool _hasTangents;

	uint32 _vertexCount;
	uint32 _indexCount;
    uint32 _numberOfTextureCoords;
    uint32 _numberOfNormals;
    uint32 _numberOfTriangles;
    uint32 _numberOfMaterials;
    uint32 _numberOfMeshes;

	GVector3 _center;
    Real _width;
    Real _height;
    Real _length;
    Real _radius;

	GString _directoryPath;

	GVector<ModelsStructs::Mesh>			_meshes;
	GVector<ModelsStructs::Material>		_materials;
	GVector<ModelsStructs::ObjModel_Vertex>	_model;		//std::vector<Vertex> m_vertexBuffer;
	GVector<Int32>		_attributeBuffer;
	GVector<Int32>		_indexBuffer;
	GVector<Real>		_vertexCoords;
	GVector<Real>		_textureCoords;
	GVector<Real>		_normals;

	std::map<GString, Int32>			_materialCache;
	std::map<Int32, GVector<Int32> >	_vertexCache;
	
	ID3D11Buffer* _pVertexBuffer, *_pIndexBuffer;
	GMeshesArray* _pMeshParts;

private:
	//prevent copy
	GModel_obj(const GModel_obj&);
	GModel_obj& operator = (const GModel_obj&);
};

#endif
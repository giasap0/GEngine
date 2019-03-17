#pragma once
#ifndef GMODEL_LOADER_H
#define GMODEL_LOADER_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include <GString.h>
#include <GVector.h>
#include <GVector3.h>
#include <GVector2.h>
using GMath::GVector3;
using GMath::GVector2;

//--------------------------------------------------------------------------------------
//Class : GModelLoader : load a 3D model from .obj file
//------------------------------------------------------------------------------------
class GModelLoader
{
private:
	struct Vertex
	{
		GVector3 pos;
		GVector2 tex;
		GVector3 norm;
	};

public:
	GModelLoader(void)			: _totalVerts(0)	{_haveNormal=_haveTexCo=false;}
	virtual ~GModelLoader(void)						{}

	void release();
	bool loadModel(const GString& fileName);

	inline const GVector<float>* getVertices()		{ return &_vertices; }
    inline const GVector<float>* getNormals()		{ return &_normals; }
    inline const GVector<float>* getTexCoords()		{ return &_texCoords; }
	inline int				 getTotalVerts() const	{ return _totalVerts; }
	inline bool				 haveNormal() const		{ return _haveNormal;}
	inline bool				 haveTexCoord() const	{ return _haveTexCo;}

private:
	//read informations from file
	bool loadOBJ(const GString& fileName);

	//functions for normalMap models with missing informations

	void calculateTangentBinormal(const Vertex&  vertex1,const Vertex&  vertex2,const Vertex&  vertex3,
											_Out_ GVector3& tangent,_Out_ GVector3& binormal) const;
	void calculateNormal(const GVector3& tangent,const GVector3& binormal, _Out_ GVector3& normal) const;

private:
	GString _directoryPath, _modelFileName, _mtlFileName;
	bool _haveNormal, _haveTexCo;
    GVector<float> _vertices;
    GVector<float> _normals;
    GVector<float> _texCoords;
	int _totalVerts;

private:
	//prevent copy
   GModelLoader(const GModelLoader&);
   GModelLoader& operator = (const GModelLoader&);
};

#endif	// GMODEL_LOADER_H
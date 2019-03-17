#pragma once
#ifndef GTMESHESARRAY_H
#define GTMESHESARRAY_H

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include "GModelLoader.h"

class GMeshPart
{
public:
	GMeshPart() :				_pTexturesArray(nullptr)		{_alpha = _shininess = 0.0f; _startIndex = _indexCount = 0;}
	virtual ~GMeshPart(void)									{ shutDown();}

	void shutDown();
	bool initialize(ID3D11Device* device, const GString& colorTexture_fileName, const GString& normalMap_fileName);

	//get

	inline const ID3D11ShaderResourceView* getColorMap() const			{return _pTexturesArray[0];}
	inline const ID3D11ShaderResourceView* getNormalMap() const			{return _pTexturesArray[1];}
	inline ID3D11ShaderResourceView*const* getTexturesArray() const		{return _pTexturesArray;}

	inline Int32 startIndex() const							{return _startIndex;}
	inline Int32 indexCount() const							{return _indexCount;}
	inline Real  shininess() const							{return _shininess;}
	inline Real  alpha() const 								{return _alpha;}
	inline GVector4 ambient() const							{return _ambient;}
	inline GVector4 diffuse() const							{return _diffuse;}
	inline GVector4 specular() const						{return _specular;}

	//set

	inline void setStartIndex(Int32 startIndex) 			{_startIndex = startIndex;}
	inline void setIndexCount(Int32 indexCount) 			{_indexCount = indexCount;}
	inline void setShininess(Real shininess)				{_shininess = shininess;}
	inline void setAlpha(Real alpha)  						{_alpha = alpha;}

	inline void setAmbient(const GVector4& ambient)			{_ambient= ambient;}
	inline void setAmbient(Real x,Real y,Real z,Real w)		{_ambient.setValues(x,y,z,w);}
	inline void setDiffuse(const GVector4& diffuse)			{_diffuse=diffuse;}
	inline void setDiffuse(Real x,Real y,Real z,Real w)		{_diffuse.setValues(x,y,z,w);}
	inline void setSpecular(const GVector4& specular)		{_specular=specular;}
	inline void setSpecular(Real x,Real y,Real z,Real w)	{_specular.setValues(x,y,z,w);}

private:
	bool initializeTextures(ID3D11Device* device, const GString& colorTexture_fileName, const GString& normalMap_fileName);

	ID3D11ShaderResourceView** _pTexturesArray;
	Int32 _startIndex;
	Int32 _indexCount;
	GVector4 _ambient;
	GVector4 _diffuse;
	GVector4 _specular;
	Real _shininess;
	Real _alpha;
};

//--------------------------------------------------------------------------------------
//Class GMeshesArray: maintain all materials informations and textures in memory
//--------------------------------------------------------------------------------------
class GMeshesArray
{
public:
	GMeshesArray(void);
	virtual ~GMeshesArray(void)			{}

	bool initialize(ID3D11Device* device, const GVector<ModelsStructs::Mesh>& meshes);
	void shutDown();

	inline void append(const GMeshPart& mp)				{_meshParts.append(mp);}

	//getters

	inline ID3D11ShaderResourceView*const* getTexturesArray(uint i) const	{return _meshParts[i].getTexturesArray();}
	inline const GMeshPart& first() const									{return _meshParts[0];}
	inline const GMeshPart& at(uint i) const								{return _meshParts.at(i);}
	inline const GMeshPart& operator[](uint i) const						{return _meshParts[i];}
	inline GMeshPart& operator[](uint i) 									{return _meshParts[i];}
	inline uint32	size() const											{return _meshParts.size();}

	inline bool isEmpty() const												{return _meshParts.isEmpty();}

private:
	GVector<GMeshPart> _meshParts;

	//prevent copy
	GMeshesArray(const GMeshesArray&);
	GMeshesArray& operator= (const GMeshesArray&);
};

#endif	//EO GTMESHESARRAY_H

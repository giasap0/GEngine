#include "GModelLoader.h"
#include <fstream>
using namespace ModelsStructs;


bool GModelLoader::loadStaticModel(const char* modelFileName, int& vertexCount, int&indexCount, BasicModelData** modelPtr)
{
	std::ifstream fin;
	fin.open(modelFileName, std::ios_base::in);
	if(fin.fail() )
	{
		DBG_OUTPT("cannot open model file "<< modelFileName);
		return false;
	}
	//read the value of the vertex count
	char input;
	fin.get(input);
	while(input!=':')
	{
		fin.get(input);
	}
	fin >> vertexCount;
	indexCount = vertexCount;
	
	//create the model
	if((*modelPtr)!=nullptr)
	{
		DBG_OUTPT("GModelLoader::loadStaticModel - deleting model data to store new values");
		delete[] (*modelPtr);
		(*modelPtr)= nullptr;
	}
	(*modelPtr)= new  BasicModelData[vertexCount];
	if((*modelPtr)==nullptr)
	{
		DBG_OUTPT("GModelLoader::loadStaticModel - cannot allocate memory for model. Vertex Count = "<< vertexCount);
		return false;
	}
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(int i=0; i<vertexCount; i++)
	{
		fin >> (*modelPtr)[i].pos.x >> (*modelPtr)[i].pos.y >> (*modelPtr)[i].pos.z;
		fin >> (*modelPtr)[i].text.x >> (*modelPtr)[i].text.y;
		fin >> (*modelPtr)[i].normal.x >> (*modelPtr)[i].normal.y >> (*modelPtr)[i].normal.z;
	}
	fin.close();
	return true;
}

bool GModelLoader::loadNormalMapModelFromStaticModel(const char* modelFileName, Int32& vertexCount, Int32& indexCount, ModelsStructs::NormalMaModelData** modelPtr)
{
	//load the basic model (with no binormal/tangent)
	BasicModelData* basicModel=nullptr;
	bool result = loadStaticModel(modelFileName, vertexCount, indexCount, &basicModel);
	if(!result)
	{
		DBG_OUTPT("GModelLoader::loadNormalMapModelFromStaticModel - failed reading from file");
		return false;
	}

	//create the model
	if((*modelPtr)!=nullptr)
	{
		DBG_OUTPT("GModelLoader::loadNormalMapModelFromStaticModel - deleting model data to store new values");
		delete[] (*modelPtr);
		(*modelPtr)= nullptr;
	}
	(*modelPtr)= new NormalMaModelData[vertexCount];
	if((*modelPtr)==nullptr)
	{
		DBG_OUTPT("GModelLoader::loadNormalMapModelFromStaticModel - cannot allocate memory for model. Vertex Count = "<< vertexCount);
		return false;
	}

	Int32 faceCount = vertexCount/3;
	Int32 index = 0;
	BasicModelData vertex1, vertex2, vertex3;
	XMFLOAT3 tangent, binormal,normal;
	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for(Int32 i=0; i<faceCount; i++)
	{
		//copy data we have for 3 vertex
		(*modelPtr)[index].pos = basicModel[index].pos;
		(*modelPtr)[index].texture = basicModel[index].text;

		(*modelPtr)[index+1].pos = basicModel[index+1].pos;
		(*modelPtr)[index+1].texture = basicModel[index+1].text;

		(*modelPtr)[index+2].pos = basicModel[index+2].pos;
		(*modelPtr)[index+2].texture = basicModel[index+2].text;

		//pass data to calculate tangent and binormal. Then calculate normal from them
		calculateTangentBinormal( basicModel[index], basicModel[index+1], basicModel[index+2],tangent,binormal);
		calculateNormal(tangent,binormal,normal);

		// Store the normal, tangent, and binormal for this face in the model structure
		(*modelPtr)[index].normal = (*modelPtr)[index+1].normal = (*modelPtr)[index+2].normal = normal;
		(*modelPtr)[index].tangent = (*modelPtr)[index+1].tangent = (*modelPtr)[index+2].tangent = tangent;
		(*modelPtr)[index].binormal = (*modelPtr)[index+1].binormal = (*modelPtr)[index+2].binormal = binormal;

		index = index+3;
	}

	
	delete[] basicModel;
	basicModel = nullptr;

	//DBG_OUTPT("GModelLoader::loadNormalMapModelFromStaticModel - loaded model with "<< vertexCount << " vertices");

	return true;
}

void GModelLoader::calculateTangentBinormal(const ModelsStructs::BasicModelData&  vertex1,const ModelsStructs::BasicModelData&  vertex2,const ModelsStructs::BasicModelData&  vertex3,
											_Out_ XMFLOAT3& tangent,_Out_ XMFLOAT3& binormal)
{
	//calculate vectors for this face
	XMFLOAT3 vector1, vector2;

	vector1.x = vertex2.pos.x - vertex1.pos.x;
	vector1.y = vertex2.pos.y - vertex1.pos.y;
	vector1.z = vertex2.pos.z - vertex1.pos.z;

	vector2.x = vertex3.pos.x - vertex1.pos.x;
	vector2.y = vertex3.pos.y - vertex1.pos.y;
	vector2.z = vertex3.pos.z - vertex1.pos.z;

	// Calculate the tu and tv texture space vectors.
	XMFLOAT2 tuVector, tvVector;

	tuVector.x = vertex2.text.x - vertex1.text.x;
	tuVector.y = vertex3.text.x - vertex1.text.x;

	tvVector.x = vertex2.text.y - vertex1.text.y;
	tvVector.y = vertex3.text.y - vertex1.text.y;

	// Calculate the denominator of the tangent/binormal equation.
	float den = 1.0f/ ( (tuVector.x * tvVector.y)  -  ( tuVector.y*tvVector.x ) );

	// Calculate the tangent and binormal
	tangent.x = ( (tvVector.y * vector1.x) - (tvVector.x * vector2.x)) * den;
	tangent.y = ( (tvVector.y * vector1.y) - (tvVector.x * vector2.y)) * den;
	tangent.z = ( (tvVector.y * vector1.z) - (tvVector.x * vector2.z)) * den;

	binormal.x = ( (tuVector.x * vector2.x) - (tuVector.y * vector1.x)) * den;
	binormal.y = ( (tuVector.x * vector2.y) - (tuVector.y * vector1.y)) * den;
	binormal.z = ( (tuVector.x * vector2.z) - (tuVector.y * vector1.z)) * den;

	float length = (tangent.x*tangent.x) + (tangent.y*tangent.y) + (tangent.z* tangent.z);
	length = GMath::reciprocalSqrt(length);

	tangent.x = tangent.x *length;
	tangent.y = tangent.y*length;
	tangent.z = tangent.z*length;

	length = (binormal.x*binormal.x) + (binormal.y*binormal.y) + (binormal.z*binormal.z);
	length = GMath::reciprocalSqrt(length);

	binormal.x = binormal.x*length;
	binormal.y = binormal.y*length;
	binormal.z = binormal.z*length;
}

void GModelLoader::calculateNormal(const XMFLOAT3& tangent,const XMFLOAT3& binormal, _Out_ XMFLOAT3& normal)
{
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	float length = (normal.x*normal.x) + (normal.y*normal.y) + (normal.z* normal.z);
	length = GMath::reciprocalSqrt(length);

	normal.x = normal.x*length;
	normal.y = normal.y*length;
	normal.z = normal.z*length;
}
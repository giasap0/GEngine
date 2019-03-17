#include "GModelLoader.h"
#include "TokenStream.h"
#include <fstream>
#include <GEngineCore.h>
#include <Windows.h>			//for debug output

bool GModelLoader::loadModel(const GString& fileName)
{
	using GMath::Int32;
	bool result = loadOBJ(fileName);
	if(!result)
	{
		DBG_OUTPT(" GModelLoader::loadModel - an error occour loading file "<< fileName.const_data() );
		return false;
	}

	//I could calculate tangent and bitangent (for normal mapping)
	/*
	if(!_haveTangent && _haveNormal && _haveTexCo)		//I can calculate tangent and bitangent
	{
		Int32 tIndex=0;
		Vertex v1,v2,v3;
		GVector3 tan, bitan, norm;

		//resize to _vertices size
		_tangent.clear();
		_tangent.resize( _vertices.size() );
		_bitangent.clear();
		_bitangent.resize( _vertices.size() );

		// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
		for(Int32 i=0; i<_totalVerts; i+=9)
		{
			//fill vertex data (_data has been stored as triangle list)
			v1.pos.setValues( _vertices[i], _vertices[i+1], _vertices[i+2] );
			v2.pos.setValues( _vertices[i+3], _vertices[i+4], _vertices[i+5] );
			v3.pos.setValues( _vertices[i+6], _vertices[i+7], _vertices[i+8] );

			v1.norm.setValues( _normals[i+0],_normals[i+1],_normals[i+2]);
			v2.norm.setValues( _normals[i+3],_normals[i+4],_normals[i+5]);
			v3.norm.setValues( _normals[i+6],_normals[i+7],_normals[i+8]);

			v1.tex.setValues( _texCoords[tIndex], _texCoords[tIndex+1] );
			v2.tex.setValues( _texCoords[tIndex+2], _texCoords[tIndex+3] );
			v3.tex.setValues( _texCoords[tIndex+4], _texCoords[tIndex+5] );
			tIndex+=6;

			//pass data to calculate tangent and binormal. Then calculate normal from them
			calculateTangentBinormal(v1,v2,v3,tan,bitan);
			calculateNormal(tan,bitan,norm);

			// Store the normal, tangent, and binormal for this face in the model structure
			_normals[i+0] = _normals[i+3] = _normals[i+6] = norm.x();
			_normals[i+1] = _normals[i+4] = _normals[i+7] = norm.y();
			_normals[i+2] = _normals[i+5] = _normals[i+8] = norm.z();

			_tangent[i+0] = _tangent[i+3] = _tangent[i+6] = tan.x();
			_tangent[i+1] = _tangent[i+4] = _tangent[i+7] = tan.y();
			_tangent[i+2] = _tangent[i+5] = _tangent[i+8] = tan.z();

			_bitangent[i+0] = _bitangent[i+3] = _bitangent[i+6] = bitan.x();
			_bitangent[i+1] = _bitangent[i+4] = _bitangent[i+7] = bitan.y();
			_bitangent[i+2] = _bitangent[i+5] = _bitangent[i+8] = bitan.z();
		}//EO for(all faces)

		_haveTangent = true;
	}//EO if(!_haveTangent && _haveNormal && _haveTexCo)
	*/
	
	return true;
}

void GModelLoader::release()
{
	_totalVerts=0;
	_texCoords.clear();
	_normals.clear();
	_vertices.clear();
	_haveNormal = _haveTexCo = false;
	_mtlFileName.clear();
	_modelFileName.clear();
	_directoryPath.clear();
}

bool GModelLoader::loadOBJ(const GString& fileName)
{
	std::ifstream fileStream;
	int fileSize =0;
	//open file and determine the size of the file in bytes

	fileStream.open(fileName.const_data(),std::ifstream::in );
	if( fileStream.is_open() == false)
	{
		DBG_OUTPT("GModelLoader::loadOBJ - cannot open model file "<< fileName.const_data() );
		return false;
	}
	_modelFileName = fileName;

	fileStream.seekg(0, std::ios::end);
	fileSize = (int)fileStream.tellg();
	fileStream.seekg(0, std::ios::beg);
	if(fileSize<=0)
		return false;

	char* buffer = new char[fileSize];
	if( buffer == nullptr )
		return false;
	memset(buffer, '\0', fileSize);

	TokenStream tokenStream, lineStream, faceStream;

	//pass informations from buffer to tokenStream
	fileStream.read(buffer, fileSize);
	tokenStream.setTokenStream(buffer);

	delete[] buffer;

	// Extract the directory the OBJ file is in from the file name.
	int position = fileName.find_last_of('/');

	if(position != fileName.lenght() || position!=-1)
	{
	_directoryPath = fileName.left(position+1);
	}
	else
	{
		position = fileName.find_last_of('\\');
		if(position != fileName.lenght() )
			_directoryPath = fileName.left(position+1);
	}

	//use lineStream to parse each individual line for the specific information we are looking for

	GVector<float> verts, norms, texC;
	GVector<int> faces;
	GVector<char> emptyVect;
	GVector<char> lineDelimiters(2);
	lineDelimiters[0]= '\n';
	lineDelimiters[1]= ' ';
	GString tempLine, token;
	GStringList mtlNames;

	while( tokenStream.moveToNextLine( &tempLine) )
	{
		lineStream.setTokenStream(tempLine);
		tokenStream.getNextToken(nullptr,emptyVect);

		if( !lineStream.getNextToken( &token, lineDelimiters) )
			continue;

		if( GString::compare( token,"v" ) == 0 )					//vertices
		{
			lineStream.getNextToken(&token,lineDelimiters);
			verts.append(token.toFloat());

			lineStream.getNextToken(&token,lineDelimiters);
			verts.append(token.toFloat());

			lineStream.getNextToken(&token,lineDelimiters);
			verts.append(token.toFloat());
		}
		else if( GString::compare( token, "vn" ) == 0)				//normals
		{
			lineStream.getNextToken(&token,lineDelimiters);
			norms.append(token.toFloat());

			lineStream.getNextToken(&token,lineDelimiters);
			norms.append(token.toFloat());

			lineStream.getNextToken(&token,lineDelimiters);
			norms.append(token.toFloat());
		}
		else if( GString::compare( token, "vt" ) == 0)				//texturesCoord
		{
			lineStream.getNextToken(&token,lineDelimiters);
			texC.append(token.toFloat());

			lineStream.getNextToken(&token,lineDelimiters);
			texC.append(token.toFloat());
		}
		else if( GString::compare( token , "mtllib") == 0)			//get name of file.mtl
		{
			lineStream.getNextToken(&token,lineDelimiters);
			_mtlFileName = _directoryPath;
			_mtlFileName.append( token );
		}
		else if( GString::compare( token, "usemtl") == 0)			//get mtl names
		{
			lineStream.getNextToken(&token,lineDelimiters);
			mtlNames.append( token );
		}
		else if( GString::compare( token, "f" ) == 0)				//faces
		{
			GVector<char> faceTokens(3);
			faceTokens[0]= '\n';
			faceTokens[1]= ' ';
			faceTokens[2]= '/';
			GString faceIndex;
			faceStream.setTokenStream( tempLine );
			faceStream.getNextToken(nullptr,emptyVect);

			for(int i=0; i<3; i++)
			{
				faceStream.getNextToken(&faceIndex, faceTokens);
				faces.append( faceIndex.toInt() );

				faceStream.getNextToken(&faceIndex, faceTokens);
				faces.append( faceIndex.toInt() );

				faceStream.getNextToken(&faceIndex, faceTokens);
				faces.append( faceIndex.toInt() );
			}
		}//EO if(faces)
		else if( GString::compare( token, "#") == 0 )				//comment
		{
			//do nothing
		}

		token[0] = '\0';
	}//EO while( tokenStream.moveToNextLine( &tempLine) )

	//we now use face information to generate a triangle list array of geometry

	int vIndex = 0, nIndex=0, tIndex=0;
	int numFaces = (int)faces.size() / 9;
	_totalVerts = numFaces*3;

	_haveNormal = ( norms.size() != 0 );
	_haveTexCo = ( texC.size() != 0 );

	//resize all vectors
	_vertices.clear();
	_vertices.resize(_totalVerts*3);

	if( _haveNormal )
	{
		_normals.clear();
		_normals.resize( _totalVerts*3);
	}
	if ( _haveTexCo )
	{
		_texCoords.clear();
		_texCoords.resize( _totalVerts*2);
	}

	for( int f=0; f<(int)faces.size(); f+=3)
	{
		_vertices[vIndex + 0] = verts[ (faces[f] -1)*3 + 0 ];
		_vertices[vIndex + 1] = verts[ (faces[f] -1)*3 + 1 ];
		_vertices[vIndex + 2] = verts[ (faces[f] -1)*3 + 2 ];
		vIndex+=3;

		if(_haveTexCo)
		{
			_texCoords[tIndex + 0] = texC[ (faces[f+1] - 1)*2 +0 ];
			_texCoords[tIndex + 0] = texC[ (faces[f+1] - 1)*2 +1 ];
			tIndex+=2;
		}
		if(_haveNormal)
		{
			_normals[ nIndex+0 ] = norms[ (faces[f+2] -1)*3 +0 ];
			_normals[ nIndex+0 ] = norms[ (faces[f+2] -1)*3 +1 ];
			_normals[ nIndex+0 ] = norms[ (faces[f+2] -1)*3 +2 ];
			nIndex+=3;
		}
	}//EO for( faces) 

	verts.clear();
	norms.clear();
	texC.clear();
	faces.clear();

	return true;
}

void GModelLoader::calculateTangentBinormal(const Vertex&  vertex1,const Vertex&  vertex2,const Vertex&  vertex3,
											_Out_ GVector3& tangent,_Out_ GVector3& binormal) const
{
	//calculate vectors for this face
	GVector3 vector1, vector2;

	vector1.x() = vertex2.pos.x() - vertex1.pos.x();
	vector1.y() = vertex2.pos.y() - vertex1.pos.y();
	vector1.z() = vertex2.pos.z() - vertex1.pos.z();

	vector2.x() = vertex3.pos.x() - vertex1.pos.x();
	vector2.y() = vertex3.pos.y() - vertex1.pos.y();
	vector2.z() = vertex3.pos.z() - vertex1.pos.z();

	// Calculate the tu and tv texture space vectors.
	GVector2 tuVector, tvVector;

	tuVector.x() = vertex2.tex.x() - vertex1.tex.x();
	tuVector.y() = vertex3.tex.x() - vertex1.tex.x();

	tvVector.x() = vertex2.tex.y() - vertex1.tex.y();
	tvVector.y() = vertex3.tex.y() - vertex1.tex.y();

	// Calculate the denominator of the tangent/binormal equation.
	float den = 1.0f/ ( (tuVector.x() * tvVector.y() )  -  ( tuVector.y()*tvVector.x() ) );

	// Calculate the tangent and binormal
	tangent.x() = ( (tvVector.y() * vector1.x() ) - ( tvVector.x() * vector2.x() )) * den;
	tangent.y() = ( (tvVector.y() * vector1.y() ) - ( tvVector.x() * vector2.y() )) * den;
	tangent.z() = ( (tvVector.y() * vector1.z() ) - ( tvVector.x() * vector2.z() )) * den;

	binormal.x() = ( (tuVector.x() * vector2.x() ) - ( tuVector.y() * vector1.x() )) * den;
	binormal.y() = ( (tuVector.x() * vector2.y() ) - ( tuVector.y() * vector1.y() )) * den;
	binormal.z() = ( (tuVector.x() * vector2.z() ) - ( tuVector.y() * vector1.z() )) * den;

	float length = (tangent.x() * tangent.x() ) + ( tangent.y() * tangent.y() ) + ( tangent.z() * tangent.z() );
	length = GMath::reciprocalSqrt(length);

	tangent.x() = tangent.x() *length;
	tangent.y() = tangent.y() *length;
	tangent.z() = tangent.z() *length;

	length = (binormal.x() * binormal.x() ) + (binormal.y() * binormal.y() ) + (binormal.z() * binormal.z() );
	length = GMath::reciprocalSqrt(length);

	binormal.x() = binormal.x() * length;
	binormal.y() = binormal.y() * length;
	binormal.z() = binormal.z() * length;
	return;
}

void GModelLoader::calculateNormal(const GVector3& tangent,const GVector3& binormal, _Out_ GVector3& normal) const
{
	normal.x() = ( tangent.y() * binormal.z() ) - ( tangent.z() * binormal.y() );
	normal.y() = ( tangent.z() * binormal.x() ) - ( tangent.x() * binormal.z() );
	normal.z() = ( tangent.x() * binormal.y() ) - ( tangent.y() * binormal.x() );

	float length = ( normal.x() * normal.x() ) + ( normal.y() * normal.y() ) + ( normal.z() * normal.z() );
	length = GMath::reciprocalSqrt(length);

	normal.x() = normal.x() * length;
	normal.y() = normal.y() * length;
	normal.z() = normal.z() * length;
	return;
}
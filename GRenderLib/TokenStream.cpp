#include "TokenStream.h"

//Helper Functions

//the only delimiters in .obj files are new lines, whitespaces, and /
bool isValidIdentifier(char c)
{
	//Ascii from ! to ~
	if( (int)c>32 && (int)c<127 )
		return true;
	return false;
}

bool isValidIdentifier(char c, const GVector<char>& delimiters)
{
	if(delimiters.isEmpty() )
		return isValidIdentifier(c);

	uint numberOfDelimiters = delimiters.size();
	for( uint i=0; i< numberOfDelimiters; i++)
	{
		if( c == delimiters.at(i) )
			return false;
	}
	return true;
}

//--------------------------------------------------------------------------------------
//This funtion will loop through the text untill it reaches a delimiter.
//Once it finds a delimiter, it uses the start index and the end index to identify a token
//This token is returned as the first parameter.
//Funtion return false if is not able to find a new token (==end of data buffer)
//--------------------------------------------------------------------------------------
bool TokenStream::getNextToken(GString* buffer, const GVector<char>& delimiters)
{
	_startIndex = _endIndex;
	//true==you are reading a string
	bool inString = false;
	int length = (int)_data.lenght();

	if(_startIndex>=length-1)			//(==end of data buffer)
		return false;

	while( _startIndex<length && isValidIdentifier( _data[_startIndex], delimiters) == false )
	{
		_startIndex++;
	}
	_endIndex = _startIndex+1;

	if(_data[_startIndex] == '"')		//inside/outside a string
		inString = !inString;
	if(_startIndex<length)
	{
		while(_endIndex<length && (isValidIdentifier(_data[_endIndex],delimiters) || inString==true) )
		{
			if(_data[_startIndex] == '"')
				inString = !inString;
			_endIndex++;
		}
		if(buffer != nullptr)
		{
			int size = (_endIndex-_startIndex);
			int index = _startIndex;

			buffer->clear();
			buffer->reserve(size+1);
			for(int i =0; i<size;i++)
			{
				buffer->append(_data[index++]);
			}
		}//EO if(buffer != nullptr)

		return true;
	}//EO if(_startIndex<length)

	return false;
}

//--------------------------------------------------------------------------------------
//This funtion will move from the current read indices to the next line of the data
//and return this line via the pointer parameter
//--------------------------------------------------------------------------------------
bool TokenStream::moveToNextLine(GString* buffer)
{
	int length = (int)_data.lenght();

	if( _startIndex< length && _endIndex<length)
	{
		_endIndex = _startIndex;
		while(_endIndex<length && (isValidIdentifier(_data[_endIndex]) || _data[_endIndex]==' ') )
		{
			_endIndex++;
		}

		if( _endIndex == _startIndex )
			return false;
		if( (_endIndex-_startIndex) >= length )
			return false;
		if( buffer != nullptr)
		{
			int size = (_endIndex-_startIndex);
			int index = _startIndex;

			buffer->clear();
			buffer->reserve(size+1);
			for(int i=0; i<size; i++)
			{
				buffer->append(_data[index++]);
			}
		}//EO if( buffer != nullptr)
	}//EO if( _startIndex< length && _endIndex<length)
	else
	{
		return false;
	}

	_endIndex++;
	_startIndex = _endIndex+1;
	return true;
}

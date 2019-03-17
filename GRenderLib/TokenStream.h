#pragma once
#ifndef TOKENSTREAM_H
#define TOKENSTREAM_H

/*************************************************************************
** Each piece of information we want to read is on its own line			**
** which means we need to write code that can parse lines of text		**
** from a file and break down the lines into smaller pieces of text.	**
** These smaller pieces of text are called tokens.						**
** This class can return to use tokens from within a text file.			**
**************************************************************************/

//--------------------------------------------------------------------------------------
//Includes
//--------------------------------------------------------------------------------------
#include <GEngineCore.h>

class TokenStream
{
public:
    TokenStream()      : _startIndex(0), _endIndex(0)		{}
    ~TokenStream()											{}

	//_startIndex = _endIndex=0;
    inline void resetStream()								{_startIndex = _endIndex=0;}
	//_startIndex = _endIndex=0; _data=data;
    inline void setTokenStream(char* data)					{resetStream(); _data = data;}
	inline void setTokenStream(const GString& data)			{resetStream(); _data=data;}

	bool getNextToken(GString* buffer, const GVector<char>& delimiters);
    bool moveToNextLine(GString* buffer);

private:
    int _startIndex, _endIndex;
    GString _data;

private:
	//prevent copy
    TokenStream(const TokenStream&);
    TokenStream& operator = (const TokenStream&);
};

#endif		//TOKENSTREAM_H
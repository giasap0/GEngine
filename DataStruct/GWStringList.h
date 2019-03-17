#pragma once
#ifndef GWSTRINGLIST_H
#define GWSTRINGLIST_H 

#include "GList.h"
#include "GWString.h"

class GWStringList: public GList<GWString>
{
public:
	GWStringList(void): GList()				{}
	GWStringList(const GWStringList& other);
	virtual ~GWStringList(void)			{}

	//Returns a list of all the strings containing the substring str
	GWStringList filter(const GWString& str) const;
	//Joins all the string list's strings into a single string with each element separated by separator
	GWString join(const GWString& separator=L"");
	GWStringList& replaceStrings(const GWString& before, const GWString& after);
	inline GWStringList operator+(const GWStringList& other) const
	{
		GWStringList copy(*this);
		copy.append(other);
		return copy;
	}
	inline GWStringList& operator+=(const GWStringList& other)
	{
		append(other);
		return *this;
	}
	GWStringList& operator+=(const GWString& other);
	GWStringList& operator<<(const GWString& str);
	inline GWStringList& operator<<(const GWStringList& other)
	{
		append(other);
		return *this;
	}
};


#endif // GWSTRINGLIST_H

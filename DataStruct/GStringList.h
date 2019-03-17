#pragma once

#ifndef GSTRINGLIST_H
#define GSTRINGLIST_H 

#include "GList.h"
#include "GString.h"

//public GList<GString>
class GStringList: public GList<GString>
{
public:
	GStringList(void): GList()			{}
	GStringList(const GStringList& other);
	virtual ~GStringList(void)			{}

	//Returns a list of all the strings containing the substring str
	GStringList filter(const GString& str) const;
	//Joins all the string list's strings into a single string with each element separated by separator
	GString join(const GString& separator="");
	GStringList& replaceStrings(const GString& before, const GString& after);
	inline GStringList operator+(const GStringList& other) const
	{
		GStringList copy(*this);
		copy.append(other);
		return copy;
	}
	inline GStringList& operator+=(const GStringList& other)
	{
		append(other);
		return *this;
	}
	GStringList& operator+=(const GString& other);
	GStringList& operator<<(const GString& str);
	inline GStringList& operator<<(const GStringList& other)
	{
		append(other);
		return *this;
	}
};
#endif // GSTRINGLIST_H
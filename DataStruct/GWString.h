#pragma once
#ifndef GWSTRING_H
#define GWSTRING_H 

#include "commonHeaders.h"

class GWStringList;

//wrapper for std::string
class GWString
{
	friend inline std::wostream& operator<<(std::wostream& os, const GWString& str)	{return operator<<(os,str._s);}
	friend inline std::wistream& operator>>(std::wistream& is, GWString& str)		{return operator>>(is,str._s);}
public:
    GWString(void) :						_s()							{}
	explicit GWString(const std::wstring str):		_s(str)					{}
    //untill null terminated character
	GWString(const wchar_t* str):			_s(str)							{}
	GWString(wchar_t ch):					_s(1,ch)						{}
	explicit GWString(const char* str);
    GWString(const GWString& other) :		_s(other._s)					{}

    virtual ~GWString(void)												{_s.clear();}

    inline void append(const GWString& str)								{_s.append(str._s);}
	inline void append(const wchar_t* str)								{size_t size = wcslen(str)+1; _s.append(str,size);}
	inline void append(const wchar_t* str, uint strLenght)				{_s.append(str,strLenght);}
    //performs a range check
    inline const wchar_t& at(uint position) const						{return _s.at(position);}
    inline uint capacity() const										{return _s.capacity();}
    //may cause reallocation
    inline void removeFromEnd(uint n)									{size_t newSize = _s.size()-n; _s.resize(newSize);}
    //delete all characters, resize to 0
    inline void clear()													{_s.clear();}
    //return a pointer to the first element
    wchar_t* data();
    inline int compare(const GWString& other) const						{return _s.compare(other._s);}
    inline int compare(const wchar_t* str) const						{return _s.compare(str);}
    bool contains (const GWString& str) const;
    bool contains(wchar_t c, uint fromPos) const;
    uint count (const GWString& str, uint pos=0) const;
    uint count (const wchar_t* str, uint pos =0) const;
    inline const wchar_t* constData() const								{return _s.data();}
    GWString& fill(wchar_t ch);
	//return position of first occurrence of char (0 = first char of the string)
	inline int find(wchar_t ch)											{return static_cast<int>(_s.find(ch));}
	//return position of first occurrence of string (0 = first char of the string)
	inline int find(const GWString& s)									{return static_cast<int>(_s.find(s._s) );}
	//return position of last occurrence of string (0 = first char of the string)
	inline int find_last_of(const GWString& s)							{return static_cast<int>(_s.find_last_of(s._s));}
	//return position of last occurrence of char (0 = first char of the string)
	inline int find_last_of(wchar_t ch)									{return static_cast<int>(_s.find_last_of(ch));}
    //insert str at position
    GWString& insert(uint position, const GWString& str);
    GWString& insert(uint position, const wchar_t* ptr_str, uint ptrStr_size);
    inline bool isEmpty() const											{return _s.empty();}
    //return a string formed by first n characters
    GWString left (uint n) const;
    //return the number of characters: the same as size()
    inline uint lenght() const											{return _s.length();}
    inline GWString& prepend(const GWString& str)						{return insert(0,str);}
    inline GWString& prepend(const wchar_t* ansi_str)					{uint x = wcslen(ansi_str); return prepend(ansi_str,x);}
    inline GWString& prepend(const wchar_t* ansi_str, uint strLenght)	{return insert(0,ansi_str,strLenght);}
    //removes number characters from the string starting from position (included)
    GWString& remove(uint position, uint number);
    //removes every occurrence of the character ch in this string
    inline GWString& remove(wchar_t ch)										{return remove(GWString(ch));}
    //removes every occurrence of the string str in this string
    GWString& remove(const GWString& str);
    //Replaces n characters beginning at index position with the string
    GWString& replace(uint position, uint n, const GWString& substitute);
    //Replaces n characters beginning at index position with the string
    GWString& replace(uint position, uint n, const wchar_t* ptr_str);
    //Replaces n characters beginning at index position with the character c
    GWString& replace(uint position, uint n, wchar_t c);
    //Replaces every occurrence of the string before with the string after
    GWString& replace(const GWString& before, const GWString& after);
    //Replaces every occurrence of the string before with the string after
    GWString& replace(const wchar_t* before, const wchar_t* after);
    inline void reserve(uint newCap)									{_s.reserve(newCap);}
    inline void resize(uint newSize)									{_s.resize(newSize);}
    //return a string formed by last n characters
    GWString right(uint n) const;
    inline uint size() const											{return _s.size();}
	//Splits the string into substrings wherever separator occurs
    GWStringList split(const GWString& separator) const;
	GWStringList split(const wchar_t* separator) const;
    GWStringList split(wchar_t separator) const;
    //release extra memory: TRY TO make capacity == size
    inline void squeeze()												{_s.shrink_to_fit();}
	//return subString starts at character position pos and spans lenght characters
	GWString subStr(uint fromPos, uint lenght) const;
    inline void swap(GWString& other)									{_s.swap(other._s);}
	inline double toDouble() const										{return std::stod(_s);}
	inline float toFloat() const										{return std::stof(_s);}
	inline int toInt() const											{return std::stoi(_s);}
	inline uint toUint() const											{return std::stoul(_s);}
    //toString cause deep copy
    inline std::wstring toStdString() const								{return _s;}
	inline const wchar_t* toC_str() const								{return _s.c_str();}
	void toCharArray(wchar_t** dest_array) const;
    //Returns a string that has whitespace removed from the start and the end
    GWString trimmed() const;

	inline GWString& operator=(const GWString& other)				{_s=other._s; return *this;}
    inline GWString& operator=(const wchar_t* other)				{_s=other; return *this;}

	inline bool operator==(const GWString& other) const				{if (_s.compare(other._s)==0) return true; return false;}
    inline bool operator==(const wchar_t* other) const				{if (_s.compare(other)==0) return true; return false;}
    inline bool operator!=(const GWString& other) const				{if (_s.compare(other._s)!=0) return true; return false;}
    inline bool operator!=(const wchar_t* other) const				{if (_s.compare(other)!=0) return true; return false;}
	inline bool operator<(const GWString& other) const				{return _s<other._s;}
	inline bool operator<(const wchar_t* other) const				{return _s<other;}
	inline bool operator<=(const GWString& other) const				{return _s<=other._s;}
	inline bool operator<=(const wchar_t* other) const				{return _s<=other;}
	inline bool operator>(const GWString& other) const				{return _s>other._s;}
	inline bool operator>(const wchar_t* other) const				{return _s>other;}
	inline bool operator>=(const GWString& other) const				{return _s>=other._s;}
	inline bool operator>=(const wchar_t* other) const				{return _s>=other;}

	inline GWString& operator+=(const GWString& other)				{_s+=other._s; return *this;}
	inline GWString& operator+=(const wchar_t* other)				{_s+=other; return *this;}
	inline GWString& operator+=(wchar_t other)						{_s+=other; return *this;}

	inline GWString operator+(const GWString& other)	const		{GWString r; r._s+=_s; r._s+=other._s; return r;}
	inline GWString operator+(const wchar_t* other) const			{GWString r; r._s+=_s; r._s+=other; return r;}
	inline GWString operator+(wchar_t other) const					{GWString r; r._s+=_s; r._s+=other; return r;}

    inline wchar_t& operator[](uint position)						{return _s[position];}
    inline const wchar_t& operator[](uint position) const			{return _s[position];}

    //statics
	
    inline static int compare(const GWString& s1, const GWString& s2)		{return s1._s.compare(s2._s);}
    inline static int compare(const wchar_t* str1, const wchar_t* str2)		{return wcscmp(str1,str2);}

	inline static GWString number(int n)								{return GWString (std::to_wstring(n));}
    inline static GWString number(const long int& n)					{return GWString (std::to_wstring(n));}
    inline static GWString number(unsigned int n)						{return GWString (std::to_wstring(n));}
    inline static GWString number (unsigned long int n)					{return GWString (std::to_wstring(n));}
    inline static GWString number(float n)								{return GWString (std::to_wstring(n));}
    inline static GWString number (const double& n)						{return GWString (std::to_wstring(n));}
	
private:
    std::wstring _s;
};

#endif // GWSTRING_H 
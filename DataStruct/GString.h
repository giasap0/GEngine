#pragma once
#ifndef GSTRING_H
#define GSTRING_H 

#include "commonHeaders.h"

//type for strings id in the hash table
typedef uint32_t StringId;

//intern the string in the hash table (return the unique stringId)
StringId internString(const wchar_t* str);

class GStringList;

//wrapper for std::string
class GString
{
	friend inline std::ostream& operator<<(std::ostream& os, const GString& str)	{return operator<<(os,str._s);}
	friend inline std::istream& operator>>(std::istream& is, GString& str)			{return operator>>(is,str._s);}
public:
    GString(void) : _s()												{}
	explicit GString(const std::string str):		_s(str)				{}
    //untill null terminated character
	GString(const char* str):			_s(str)							{}
	GString(char ch):					_s(1,ch)						{}
    GString(const GString& other) :		_s(other._s)					{}
	// This is the implicit conversion operator
	//operator const GString&() const									{return *this;}

    virtual ~GString(void)												{_s.clear();}

    inline void append(const GString& str)								{_s.append(str._s);}
    inline void append(const char* str)									{size_t size = strlen(str)+1; _s.append(str,size);}
	inline void append(const char* str, uint strLenght)					{_s.append(str,strLenght);}
    //performs a range check
    inline const char& at(uint position) const							{return _s.at(position);}
    inline uint capacity() const										{return _s.capacity();}
    //may cause reallocation
    inline void removeFromEnd(uint n)									{size_t newSize = _s.size()-n; _s.resize(newSize);}
    //delete all characters, resize to 0
    inline void clear()													{_s.clear();}
    //return a pointer to the first element
    char* data();
    inline int compare(const GString& other) const						{return _s.compare(other._s);}
    inline int compare(const char* str) const							{return _s.compare(str);}
	inline const char* const_data() const								{return _s.data();}
    bool contains (const GString& str) const;
    bool contains(char c, uint fromPos) const;
    uint count (const GString& str, uint pos=0) const;
    uint count (const char* str, uint pos =0) const;
    GString& fill(char ch);
	//return position of first occurrence of char (0 = first char of the string)
	inline int find(char ch) const										{return static_cast<int>(_s.find(ch));}
	//return position of first occurrence of string (0 = first char of the string)
	inline int find(const GString& s) const								{return static_cast<int>(_s.find(s._s));}
	//return position of last occurrence of string (0 = first char of the string)
	inline int find_last_of(const GString& s) const						{return static_cast<int>(_s.find_last_of(s._s));}
	//return position of last occurrence of char (0 = first char of the string)
	inline int find_last_of(char ch) const								{return static_cast<int>(_s.find_last_of(ch));}
    //insert str at position
    GString& insert(uint position, const GString& str);
    GString& insert(uint position, const char* ptr_str, uint ptrStr_size);
    inline bool isEmpty() const											{return _s.empty();}
    //return a string formed by first n characters
    GString left (uint n) const;
    //return the number of characters: the same as size()
    inline uint lenght() const											{return _s.length();}
    inline GString& prepend(const GString& str)							{return insert(0,str);}
    inline GString& prepend(const char* ansi_str)						{uint x = strlen(ansi_str); return prepend(ansi_str,x);}
    inline GString& prepend(const char* ansi_str, uint strLenght)		{return insert(0,ansi_str,strLenght);}
    //removes number characters from the string starting from position (included)
    GString& remove(uint position, uint number);
    //removes every occurrence of the character ch in this string
    inline GString& remove(char ch)										{return remove(GString(ch));}
    //removes every occurrence of the string str in this string
    GString& remove(const GString& str);
    //Replaces n characters beginning at index position with the string
    GString& replace(uint position, uint n, const GString& substitute);
    //Replaces n characters beginning at index position with the string
    GString& replace(uint position, uint n, const char* ptr_str);
    //Replaces n characters beginning at index position with the character c
    GString& replace(uint position, uint n, char c);
    //Replaces every occurrence of the string before with the string after
    GString& replace(const GString& before, const GString& after);
    //Replaces every occurrence of the string before with the string after
    GString& replace(const char* before, const char* after);
	//trim the string and replace multiple spaces and tabs with one space
	GString& cleanSpaces();
    inline void reserve(uint newCap)									{_s.reserve(newCap);}
    inline void resize(uint newSize)									{_s.resize(newSize);}
    //return a string formed by last n characters
    GString right(uint n) const;
    inline uint size() const											{return _s.size();}
	//Splits the string into substrings wherever separator occurs
    GStringList split(const GString& separator) const;
	GStringList split(const char* separator) const;
    GStringList split(char separator) const;
    //release extra memory: TRY TO make capacity == size
    inline void squeeze()												{_s.shrink_to_fit();}
	//return subString starts at character position pos and spans lenght characters
	GString subStr(uint fromPos, uint lenght) const;
    inline void swap(GString& other)									{_s.swap(other._s);}
	inline double toDouble() const										{return std::stod(_s);}
	inline float toFloat() const										{return std::stof(_s);}
	inline int toInt() const											{return std::stoi(_s);}
	inline uint toUint() const											{return std::stoul(_s);}
    //toString cause deep copy
    inline std::string toStdString() const								{return _s;}
	inline const char* toC_str() const									{return _s.c_str();}
	void toCharArray( _Out_ char** dest_array) const;
    //Returns a string that has whitespace removed from the start and the end
	//TODO FIX BUG - now he removes all spaces
    GString trimmed() const;

	inline GString& operator=(const GString& other)						{_s=other._s; return *this;}
    inline GString& operator=(const char* other)						{_s=other; return *this;}

	inline bool operator==(const GString& other) const				{if (_s.compare(other._s)==0) return true; return false;}
    inline bool operator==(const char* other) const					{if (_s.compare(other)==0) return true; return false;}
    inline bool operator!=(const GString& other) const				{if (_s.compare(other._s)!=0) return true; return false;}
    inline bool operator!=(const char* other) const					{if (_s.compare(other)!=0) return true; return false;}
	inline bool operator<(const GString& other) const				{return _s<other._s;}
	inline bool operator<(const char* other) const					{return _s<other;}
	inline bool operator<=(const GString& other) const				{return _s<=other._s;}
	inline bool operator<=(const char* other) const					{return _s<=other;}
	inline bool operator>(const GString& other) const				{return _s>other._s;}
	inline bool operator>(const char* other) const					{return _s>other;}
	inline bool operator>=(const GString& other) const				{return _s>=other._s;}
	inline bool operator>=(const char* other) const					{return _s>=other;}

	inline GString& operator+=(const GString& other)					{_s+=other._s; return *this;}
	inline GString& operator+=(const char* other)						{_s+=other; return *this;}
	inline GString& operator+=(char other)								{_s+=other; return *this;}

	inline GString operator+(const GString& other)	const			{GString r; r._s+=_s; r._s+=other._s; return r;}
	inline GString operator+(const char* other) const				{GString r; r._s+=_s; r._s+=other; return r;}
	inline GString operator+(char other) const						{GString r; r._s+=_s; r._s+=other; return r;}

    inline char& operator[](uint position)								{return _s[position];}
    inline const char& operator[](uint position) const					{return _s[position];}

    //statics
	
    inline static int compare(const GString& s1, const GString& s2)		{return s1._s.compare(s2._s);}
    inline static int compare(const char* str1, const char* str2)		{return strcmp(str1,str2);}

	inline static GString number(int n)									{return GString (std::to_string(n));}
    inline static GString number(const long int& n)						{return GString (std::to_string(n));}
    inline static GString number(unsigned int n)						{return GString (std::to_string(n));}
    inline static GString number (unsigned long int n)					{return GString (std::to_string(n));}
    inline static GString number(float n)								{return GString (std::to_string(n));}
    inline static GString number (const double& n)						{return GString (std::to_string(n));}
	
private:
    std::string _s;
};

#endif // GSTRING_H 
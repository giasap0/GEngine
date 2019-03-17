#include "commonHeaders.h"
#include "GStringList.h"


/*
static HashTable<StringId, const wchar_t*> g_StringIdTable;
typedef HashTable<StringId, const wchar_t*>::iterator HashIterator;

StringId internString(const wchar_t* str)
{
	StringId s_id = hashCrc32(str);						//create unique hash (an int) for every string
	HashIterator it	= g_StringIdTable.find(s_id);		//to find an int is faster than finding a string
	if (it == gStringTable.end())
		{
		// This string has not yet been added to the table. 
		//Add it, being sure to copy it in case
		// the original was dynamically allocated and might later be freed.
		gStringTable[sid] = strdup(str);
		}
	return s_id;
}
*/

char* GString::data()
{
    std::string::iterator it = _s.begin();
	return &(*it);
}
bool GString::contains(const GString& str) const
{
    std::string::size_type r = _s.find(str._s,0);
    if(r==_s.npos)
        return false;
    return true;
}
bool GString::contains(char c, uint fromPos) const
{
    if(fromPos>=_s.size())
        return false;
	std::string::size_type r = _s.find(c,fromPos);
    if(r==_s.npos)
        return false;
    return true;
}
uint GString::count(const GString& str, uint pos) const
{
    std::string::size_type sSize = _s.size();
    if(pos>=sSize)
        return 0;
    std::string::size_type lastPos = pos;
    uint _count=0;
    if(pos==0)
    {
		lastPos = _s.find(str._s,pos);
        if(lastPos!=_s.npos)
            _count++;
    }
    while(lastPos!=_s.npos && lastPos+1<sSize)
    {
        lastPos = _s.find(str._s,lastPos+1);
        if(lastPos!=_s.npos)
            _count++;
    }
    return _count;
}

uint GString::count(const char* str, uint pos) const
{
	std::string::size_type sSize = _s.size();
	if(pos>=sSize)
		return 0;
	std::string::size_type lastPos = pos;
	uint _count=0;
    if(pos==0)
	{
		lastPos = _s.find(str,pos);
		if(lastPos!=_s.npos)
			_count++;
	}
	while(lastPos!=_s.npos && lastPos+1<sSize)
    {
        lastPos = _s.find(str,lastPos+1);
        if(lastPos!=_s.npos)
            _count++;
    }
	return _count;
}
GString& GString::fill(char ch)
{
    std::string::size_type sSize = _s.size();
	_s.replace(_s.begin(),_s.begin()+sSize,sSize,ch);
    return *this;
}

GString& GString::insert(uint position, const GString& str)
{
	_s.insert(position,str._s);
    return *this;
}

GString& GString::insert(uint position, const char* ptr_str, uint ptrStr_size)
{
	_s.insert(position,ptr_str,ptrStr_size);
    return *this;
}
GString GString::left(uint n) const
{
    GString newString;
    newString._s = _s.substr(0,n);
    return newString;
}
GString& GString::remove(uint position, uint number)
{
    _s.erase(position,number);
    return *this;
}
GString& GString::remove(const GString& str)
{
    std::string::size_type sSize = _s.size();
    std::string::size_type clearSize = str.size();
    std::string::size_type lastPos = 0;

    while (lastPos!=_s.npos && lastPos+1<sSize)
    {
        lastPos = _s.find(str._s,lastPos);
        if(lastPos!=_s.npos)
        {
            _s.erase(lastPos,clearSize);
        }
    }
    return *this;
}
GString& GString::replace(uint position, uint n, const GString& substitute)
{
    _s.erase(position,n);
	_s.insert(position,substitute._s);
    return *this;
}
GString& GString::replace(uint position, uint n, const char* ptr_str)
{
    _s.erase(position,n);
	_s.insert(position,ptr_str,strlen(ptr_str) );
    return *this;
}
GString& GString::replace(uint position, uint n, char c)
{
    _s.erase(position,n);
	_s.insert(position, 1,c);	//Inserts a string formed by a repetition of character c, n times, at position pos1.
    return *this;
}
GString& GString::replace(const GString& before, const GString& after)
{
    std::string::size_type clearSize = before.size();
    std::string::size_type otherSize = after.size();
    std::string::size_type position =  _s.find(before._s,0);

    while (position!=_s.npos && position< _s.size())					//_s.size() changes
    {
        _s.erase(position,clearSize);
		_s.insert(position,after._s);
		position = _s.find(before._s,position+otherSize);
    }
    return *this;
}
GString& GString::replace(const char* before, const char* after)
{
	std::string::size_type clearSize = strlen(before);
	std::string::size_type afterSize = strlen(after);
	std::string::size_type position =  _s.find(before,0,clearSize);

	while (position!=_s.npos && position< _s.size())					//_s.size() changes
    {
        _s.erase(position,clearSize);
		_s.insert(position,after,afterSize);
		position = _s.find(before,position+afterSize,clearSize);
    }
	return *this;
}

GString GString::right(uint n) const
{
	GString r;
	if(n==0)
		return r;
	std::string::size_type sSize = _s.size();
	if(n>sSize)
		n= sSize;
	std::string::size_type position = sSize-n;
	r._s.resize(n);
	for(uint i=0; i<n; i++)
	{
		r._s[i] = _s[position+i];
	}
	return r;
}

GStringList GString::split(const GString& separator) const
{
	GStringList L;
	std::string::size_type ssize = _s.size();
	std::string::size_type separatorSize = separator.size();
	std::string::size_type position = _s.find(separator._s,0);
	std::string::size_type from = 0;

	while(position!=_s.npos && position<ssize )
	{
		L.append( GString(_s.substr(from,position-from) ) );
		from = position+separatorSize;
		position = _s.find(separator._s,from+1);
	}
	L.append( GString(_s.substr(from,ssize)));
	return L;
}
/*
uint GString::count(const GString& str, uint pos) const
{
    std::string::size_type sSize = _s.size();
    if(pos>=sSize)
        return 0;
    std::string::size_type lastPos = pos;
    uint _count=0;
    if(pos==0)
    {
		lastPos = _s.find(str._s,pos);
        if(lastPos!=_s.npos)
            _count++;
    }
    while(lastPos!=_s.npos && lastPos+1<sSize)
    {
        lastPos = _s.find(str._s,lastPos+1);
        if(lastPos!=_s.npos)
            _count++;
    }
    return _count;
}
*/
GStringList GString::split(const char* separator) const
{
	return split(GString(separator));
}
GStringList GString::split(char separator) const
{
	return split(GString(separator));
}

GString GString::subStr(uint fromPos, uint lenght) const
{
	GString sub( _s.substr(static_cast<uint>(fromPos), static_cast<uint>(lenght) ) );
	return sub;
}

void GString::toCharArray(char** dest_array) const
{
    if(*dest_array!=nullptr)
        return;
    std::string::size_type sSize = _s.size();
    *dest_array = new char[sSize];
    for(uint i=0; i<sSize; i++)
    {
        *(*dest_array+i) = _s.at(i);
    }
    return;
}
GString GString::trimmed() const
{
	GString r;
	std::string::size_type sSize = _s.size();
	r._s.reserve(sSize);
	for(uint i=0; i<sSize; i++)
	{
		if(_s.at(i)!=' ')
			r._s.append(1,_s.at(i));
	}
	r._s.shrink_to_fit();
	return r;
}


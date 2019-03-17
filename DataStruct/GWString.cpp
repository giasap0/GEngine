#include "commonHeaders.h"
#include "GWStringList.h"

GWString::GWString(const char* str) : _s()
{
	wchar_t* wstr = nullptr;
	size_t origsize = strlen(str) + 1;
	size_t convertedChars = 0;
	wstr = new wchar_t[origsize];
	mbstowcs_s(&convertedChars,wstr,origsize,str,origsize);

	_s = std::wstring(wstr);
	delete[] wstr;
	wstr= nullptr;
}


wchar_t* GWString::data()
{
    std::wstring::iterator it = _s.begin();
	return &(*it);
}
bool GWString::contains(const GWString& str) const
{
    std::wstring::size_type r = _s.find(str._s,0);
    if(r==_s.npos)
        return false;
    return true;
}
bool GWString::contains(wchar_t c, uint fromPos) const
{
    if(fromPos>=_s.size())
        return false;
	std::wstring::size_type r = _s.find(c,fromPos);
    if(r==_s.npos)
        return false;
    return true;
}
uint GWString::count(const GWString& str, uint pos) const
{
    std::wstring::size_type sSize = _s.size();
    if(pos>=sSize)
        return 0;
    std::wstring::size_type lastPos = pos;
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

uint GWString::count(const wchar_t* str, uint pos) const
{
	std::wstring::size_type sSize = _s.size();
	if(pos>=sSize)
		return 0;
	std::wstring::size_type lastPos = pos;
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
GWString& GWString::fill(wchar_t ch)
{
    std::wstring::size_type sSize = _s.size();
	_s.replace(_s.begin(),_s.begin()+sSize,sSize,ch);
    return *this;
}

GWString& GWString::insert(uint position, const GWString& str)
{
	_s.insert(position,str._s);
    return *this;
}

GWString& GWString::insert(uint position, const wchar_t* ptr_str, uint ptrStr_size)
{
	_s.insert(position,ptr_str,ptrStr_size);
    return *this;
}
GWString GWString::left(uint n) const
{
    GWString newString;
    newString._s = _s.substr(0,n);
    return newString;
}
GWString& GWString::remove(uint position, uint number)
{
    _s.erase(position,number);
    return *this;
}
GWString& GWString::remove(const GWString& str)
{
    std::wstring::size_type sSize = _s.size();
    std::wstring::size_type clearSize = str.size();
    std::wstring::size_type lastPos = 0;

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
GWString& GWString::replace(uint position, uint n, const GWString& substitute)
{
    _s.erase(position,n);
	_s.insert(position,substitute._s);
    return *this;
}
GWString& GWString::replace(uint position, uint n, const wchar_t* ptr_str)
{
    _s.erase(position,n);
	_s.insert(position,ptr_str,wcslen(ptr_str) );
    return *this;
}
GWString& GWString::replace(uint position, uint n, wchar_t c)
{
    _s.erase(position,n);
	_s.insert(position, 1,c);	//Inserts a string formed by a repetition of character c, n times, at position pos1.
    return *this;
}
GWString& GWString::replace(const GWString& before, const GWString& after)
{
    std::wstring::size_type clearSize = before.size();
    std::wstring::size_type otherSize = after.size();
    std::wstring::size_type position =  _s.find(before._s,0);

    while (position!=_s.npos && position< _s.size())					//_s.size() changes
    {
        _s.erase(position,clearSize);
		_s.insert(position,after._s);
		position = _s.find(before._s,position+otherSize);
    }
    return *this;
}
GWString& GWString::replace(const wchar_t* before, const wchar_t* after)
{
	std::wstring::size_type clearSize = wcslen(before);
	std::wstring::size_type afterSize = wcslen(after);
	std::wstring::size_type position =  _s.find(before,0,clearSize);

	while (position!=_s.npos && position< _s.size())					//_s.size() changes
    {
        _s.erase(position,clearSize);
		_s.insert(position,after,afterSize);
		position = _s.find(before,position+afterSize,clearSize);
    }
	return *this;
}

GWString GWString::right(uint n) const
{
	GWString r;
	if(n==0)
		return r;
	std::wstring::size_type sSize = _s.size();
	if(n>sSize)
		n= sSize;
	std::wstring::size_type position = sSize-n;
	r._s.resize(n);
	for(uint i=0; i<n; i++)
	{
		r._s[i] = _s[position+i];
	}
	return r;
}

GWStringList GWString::split(const GWString& separator) const
{
	GWStringList L;
	std::wstring::size_type ssize = _s.size();
	std::wstring::size_type separatorSize = separator.size();
	std::wstring::size_type position = _s.find(separator._s,0);
	std::wstring::size_type from = 0;

	while(position!=_s.npos && position<ssize )
	{
		L.append(GWString(_s.substr(from,position-from)));
		from = position+separatorSize;
		position = _s.find(separator._s,from+1);
	}
	L.append(GWString(_s.substr(from,ssize)));
	return L;
}
/*
uint GWString::count(const GWString& str, uint pos) const
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
GWStringList GWString::split(const wchar_t* separator) const
{
	return split(GWString(separator));
}
GWStringList GWString::split(wchar_t separator) const
{
	return split(GWString(separator));
}
GWString GWString::subStr(uint fromPos, uint lenght) const
{
	GWString sub( _s.substr(static_cast<uint>(fromPos), static_cast<uint>(lenght) ) );
	return sub;
}
void GWString::toCharArray(wchar_t** dest_array) const
{
    if(*dest_array!=nullptr)
        return;
    std::wstring::size_type sSize = _s.size();
    *dest_array = new wchar_t[sSize];
    for(uint i=0; i<sSize; i++)
    {
        *(*dest_array+i) = _s.at(i);
    }
    return;
}
GWString GWString::trimmed() const
{
	GWString r;
	std::wstring::size_type sSize = _s.size();
	r._s.reserve(sSize);
	for(uint i=0; i<sSize; i++)
	{
		if(_s.at(i)!=' ')
			r._s.append(1,_s.at(i));
	}
	r._s.shrink_to_fit();
	return r;
}


#include "commonHeaders.h"
#include "GStringList.h"


GStringList::GStringList(const GStringList& other)
{
	auto it = other.const_begin();
	while(it!=other.const_end() )
	{
		append(*it);
		++it;
	}
}
GString GStringList::join(const GString& separator) 
{
		auto it = const_begin();
		GString output;
		while(it!=const_end() )
		{	
			output.append(*it+separator);
			++it;
		}
		return output;
}
GStringList& GStringList::replaceStrings(const GString& before, const GString& after)
{
	auto it = begin();
		while (it!=end())
		{
			if(*it==before)
			{
				*it=after;
			}
			++it;
		}
	return *this;
}
GStringList& GStringList::operator+=(const GString& other)
{
		append(other);
		return *this;
}
GStringList& GStringList::operator<<(const GString& str)
{
	append(str);
	return *this;
}
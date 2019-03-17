#include "commonHeaders.h"
#include "GWStringList.h"


GWStringList::GWStringList(const GWStringList& other)
{
	auto it = other.const_begin();
	while(it!=other.const_end() )
	{
		append(*it);
		++it;
	}
}
GWString GWStringList::join(const GWString& separator) 
{
		auto it = const_begin();
		GWString output;
		while(it!=const_end() )
		{	
			output.append(*it+separator);
			++it;
		}
		return output;
}
GWStringList& GWStringList::replaceStrings(const GWString& before, const GWString& after)
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
GWStringList& GWStringList::operator+=(const GWString& other)
{
		append(other);
		return *this;
}
GWStringList& GWStringList::operator<<(const GWString& str)
{
	append(str);
	return *this;
}
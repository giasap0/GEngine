#pragma once
#ifndef GLIST_H
#define GLIST_H 

#include <stdint.h>
#include <vector>
#include <list>
#include <iostream>

typedef uint32_t uint;

template<class T> class GList
{
public:
	GList(void)												{}
	GList(const GList& other) : _list(other._list)			{}
	virtual ~GList(void)									{}

	inline void		append(const T& value)									{_list.push_back(value);}
			void	append(const GList<T>& other);
	//Don't use at() in loops!
	const	T&		at(uint i) const;
	inline typename std::list<T>::iterator begin()							{return _list.begin();}
	inline typename std::list<T>::const_iterator const_begin() const		{return _list.cbegin();}
	inline void		clear()													{_list.clear();}
	//O(n), return the first element with value==value
			bool	contains(const T& value, uint fromPosition=0U) const;
			bool	contains(const T&value, typename std::list<T>::const_iterator fromPosition) const;
			uint	count(const T& value) const;
	inline typename std::list<T>::iterator end()							{return _list.end();}
	inline typename std::list<T>::const_iterator const_end() const			{return _list.cend();}
			//don't use erase(uint) in loops, use erase(iterator)
			void	erase(uint position);
	inline  void	erase(typename std::list<T>::const_iterator position)	{_list.erase(position);}
			void	erase(uint fromPosition, uint toPosition);
	inline T&		first()													{return _list.front();}
	inline const T& first() const											{return _list.front();}
	//if no value return -1
			uint	indexOf(const T& value, uint from=0) const;
	inline	void	insert(const T& value, typename std::list<T>::const_iterator position)	{_list.insert(position,value);}
			void	insert (const T& value, uint position);
	inline	bool	isEmpty() const											{return _list.empty();}
	//first calls contains(value,fromIndex) to ensure value is present
	typename std::list<T>::const_iterator iteratorOf(const T& value, uint from=0) const;
	//first calls contains(value,fromIterator) to ensure value is present
	typename std::list<T>::const_iterator iteratorOf(const T& value, typename std::list<T>::const_iterator from) const;
	inline	T&		last()													{return _list.back();}
	inline const T& last() const											{return _list.back();}
	inline	uint	lastIndex() const
	{
		uint indx=0;
		auto it=_list.cbegin();
		while(it!=_list.cend() )
		{
			++indx;
			++it;
		}
		return indx;
	}
			uint	lastIndexOf(const T& value) const;
			void	moveElements(uint position1, uint position2);
			void	moveFrom(GList<T>&thisList, uint moveInThisPosition);
	inline	void	moveFrom(GList<T>& fromList, typename std::list<T>::const_iterator moveInThisPosition)	{_list.splice(moveInThisPosition,fromList._list);}
			void	moveFrom(GList<T>& fromList, uint elementPosition, uint moveInThisPosition);
	inline	void	prepend(const T& value)									{_list.push_front(T);}
	//Removes all occurrences of value in the list and returns the number of entries removed
			uint	removeAll(const T& valuetoRemove);
			void	removeAt ( uint indx );
	inline	void	removeAt(typename std::list<T>::const_iterator position){_list.erase(position);}
	inline	void	removeFirst ()											{_list.pop_front();}
	inline	void	removeLast ()											{_list.pop_back();}
	inline	void	removeDuplicates()										{_list.unique();}
			void	replace(uint position, const T& newValue);
	inline	void	reverse()												{_list.reverse();}
	inline	void	sort()													{_list.sort();}
	inline	uint	size() const											{return _list.size();}
	inline	std::list<T> toStdList() const									{return _list;}
			T		value(uint position) const;
			T		value (uint position, const T& defaultValue) const;

	inline	GList<T>&	operator= (const GList<T>& other)					{_list=other._list; return *this;}

			GList<T>	operator+(const GList<T>& other) const;
			GList<T>	operator+(const T& appendMe) const;
	inline	GList<T>&	operator+=(const GList<T>& other)					{append(other); return *this;}
	inline	GList<T>&	operator+=(const T& appendMe)						{append(appendMe); return *this;}

private:
	std::list<T> _list;
};

template<class T> void GList<T>::append(const GList<T>& other)
{
	auto it = other._list.begin();
	while(it!= other._list.end() )
	{
		_list.push_back(*(it));
		++it;
	}
}

template<class T> const T& GList<T>::at(uint i) const
{
	std::list<T>::const_iterator it = _list.cbegin();
	for(uint x=1; x<=i; x++)
		++it;
	return *(it);
}
template<class T> bool GList<T>::contains(const T& value, uint fromPosition) const
{
	bool found= false;
	auto iter = _list.cbegin()
	for(uint i=0; i<fromPosiotn; ++i)
		++iter;

	while(found==false && iter!=_list.cend() )
	{
		if(*(iter)==value)
			found = true;
		++iter;
	}
	return found;
}
template<class T>
bool GList<T>::contains(const T& value, typename std::list<T>::const_iterator fromPosition) const
{
	bool found = false;
	auto iter = fromPosition;
	while(found==false && iter!=_list.cend() )
	{
		if(*(iter)==value)
			found = true;
		++iter;
	}
	return found;
}

template<class T> uint GList<T>::count(const T& value) const
{
	uint c=0;
	for(std::list<T>::const_iterator iter = _list.cbegin(); iter != _list.cend(); ++iter)
	{
		if(*(iter)==value)
			c++;
	}
	return c;
}

template<class T> void GList<T>::erase(uint position)
{
	std::list<T>::const_iterator iter = _list.cbegin();
	for(uint i=0; i<position; ++i)
		++iter;
	_list.erase(iter);
}
template<class T> void GList<T>::erase(uint fromPosition, uint toPosition)
{
	std::list<T>::const_iterator end, begin = _list.cbegin();

	for(uint i=0; i<fromPosition; ++i)
		++begin;
	end = begin;
	for(uint i=fromPosition; i<=toPosition; ++i)
		++end;
	_list.erase(begin,end);
}

template <class T> uint GList<T>::indexOf(const T& value, uint from) const
{
	uint indx=0;
	std::list<T>::const_iterator iterator = _list.cbegin();
	if(from==0 && *iterator==value)
		return 0;
	for(uint i=0; i<from; ++i)
		++iterator;
	indx=from;
	//now search for value
	bool found= false;
	while(iterator!=_list.cend() && found ==false)
	{
		if(*iterator == value)
		{
			found = true;
			break;
		}
		++iterator;
		++indx;
	}
	if(found == false)
		return -1;
	return indx;
}
template<class T> void GList<T>::insert(const T& value, uint position)
{
	std::list<T>::const_iterator iterator = _list.cbegin();
	for(uint i=0; i<position; ++i)
		++iterator;
	_list.insert(iterator,value);
}
template<class T>
typename std::list<T>::const_iterator GList<T>::iteratorOf(const T& value, uint from) const
{
	auto iterator = _list.cbegin();
	if(from==0 && *iterator==value)
		return iterator;
	for(uint i=0; i<from; ++i)
		++iterator;
	//now search for value
	bool found= false;
	while(iterator!=_list.cend() && found ==false)
	{
		if(*iterator == value)
		{
			found = true;
			break;
		}
		++iterator;
	}
	return iterator;
}
template<class T>
typename std::list<T>::const_iterator GList<T>::iteratorOf(const T& value, typename std::list<T>::const_iterator from) const
{
	auto iterator = from;
	if(*iterator==value)
		return iterator;
	bool found = false;
	while(iterator!=_list.cend() && found ==false)
	{
		if(*iterator == value)
		{
			found = true;
			break;
		}
		++iterator;
	}
	return iterator;
}
template<class T> uint GList<T>::lastIndexOf(const T& value) const
{
	uint indx = -1, counter=0;
	auto iterator = _list.cbegin();
	if(*iterator==value)
		return 0;
	while(iterator!=_list.cend() )
	{
		if(*iterator==value)
		{
			indx=counter;
		}
		++counter;
		++iterator;
	}
	return indx;
}
template<class T> void GList<T>::moveElements(uint position1, uint position2)
{
	std::list<T>::iterator it1 = _list.begin();
	std::list<T>::iterator it2 = _list.begin();
	for(uint i=0; i<position1;++i)
		++it1;
	for(uint i=0; i<position2;++i)
		++it2;
	T temp = *it2;
	*it2 = *it1;
	*it1 = temp;
}
template<class T> void GList<T>::moveFrom(GList<T>& thisList, uint moveInThisPosition)
{
	std::list<T>::iterator it = _list.begin();
	for(uint i=0; i<moveInThisPosition;i++)
		++it;
	_list.splice(it,thisList._list);
}
template<class T> void GList<T>::moveFrom(GList<T>& fromList, uint elementPosition, uint moveInThisPosition)
{
	std::list<T>::iterator it = _list.begin();
	for(uint i=0; i<moveInThisPosition;i++)
		++it;
	std::list<T>::iterator fromIterator = fromList.begin();
	for(uint i=0; i<elementPosition; i++)
		++fromIterator;

	_list.splice(it,fromList._list,fromIterator);
}
template<class T> uint GList<T>::removeAll(const T& valueToRemove)
{
	uint n = count(valueToRemove);
	_list.remove(valueToRemove);
	return n;
}
template<class T> void GList<T>::removeAt(uint indx)
{
	auto it = _list.cbegin();
	for(uint i=0; i<indx;++i)
		++it;
	_list.erase(it);
}
template<class T> void GList<T>::replace(uint position, const T& newValue)
{
	auto it = _list.begin();
	for(uint i=0; i<position;++i)
		++it;
	*it=newValue;
}
template<class T> T GList<T>::value(uint position) const
{
	if(position>lastIndex())
		return T();
	auto it = _list.cbegin();
	for(uint i=0; i<position;++i)
		++it;
	return *it;
}
template<class T> T GList<T>::value(uint position, const T& defaultValue) const
{
	if(position>lastIndex())
		return defaultValue;
	auto it = _list.cbegin();
	for(uint i=0; i<position;++i)
		++it;
	return *it;
}
template<class T> GList<T> GList<T>::operator+(const GList<T>& other) const
{
	GList<T> copy(*this);
	copy.append(other);
	return copy;
}
template<class T> GList<T> GList<T>::operator+(const T& appendMe) const
{
	GList<T>copy(*this);
	copy.append(appendMe);
	return copy;
}
/*
			GList<T>&	operator+=(const GList<T>& other);
			GList<T>&	operator+=(const T& appendMe);
*/

#endif // GLIST_H 
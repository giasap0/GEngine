#pragma once
#ifndef GVECTOR_H
#define GVECTOR_H 

#include <vector>
#include <algorithm>

typedef unsigned int uint;

//-------------------------------------
//class GVector: wrapper for std:vector
//-------------------------------------
template<class T> class GVector
{
public:
					GVector(void)						:_v(0)				{}
	explicit		GVector(uint size);
					GVector(uint size, const T& value);
					GVector(const GVector<T>& other);
					virtual	~GVector(void)									{ }

	//insert value at last position
	inline void		append(const T& value)									{_v.push_back(value);}
	//return constant reference to element in position i
	const T&		at(uint i) const;
	int				capacity() const;
	//clear all the elements in the vector and resize it to 0
	void			clear();	
	bool			contains(const T& value) const;
	//return number of vector's elements with value==value
	int				count(const T& value) const;
	//return pointer to the first element
	T*				data();
	//fill the vector with value value and if size!=-1 resize to size
	GVector<T>&		fill(const T& value, int size = -1);
	//return reference to first element
	T&				first();
	//return position of element with value value. if it doesn't exists return -1
	int				indexOf(const T& value, uint from=0) const;
	//insert value in position = index
	void			insert(uint index, const T& value);
	//insert number elements with value==value, starting from position index
	void			insert(uint index, uint number, const T& value);
	//true if ther's no elements
	bool			isEmpty() const;
	//reference to last element
	T&				last();
	//the maximum potential size due to system
	inline uint		max_size()							{return _v.max_size();}
	//insert element with value value at position 0
	void			prepend(const T& value);
	//remove element at index==atIndx and resize vector to size-1
	void			remove(uint atIndx);
	//remove number of lemenets starting from index atIndex (included), also resize vector
	void			remove(uint atIndx, uint number);
	void			replace(uint atIndx, const T& value);
	void			reserve(uint n);
	void			resize(uint newSize);
	//p_compare = a function pointer that return true if left<right
	inline void		sort(bool(*p_compare)(T,T))			{std::sort(_v.begin(),_v.end(),p_compare);}
	uint			size() const;
	//release extra memory: TRY TO make capacity == size
	void			squeeze ();
	void			swap ( GVector<T> & other );
	std::vector<T>	toStdVector () const;
	//return the value of the element in position atIndx. if indx is not valid return default constuction
	T				value ( uint atIndx ) const;
	//return the value of the element in position atIndx, if indx is not valid return defaultValue
	T				value ( uint atIndx, const T & defaultValue ) const;

	virtual GVector<T> &	operator= ( const GVector<T> & other );
	bool			operator == (const GVector<T>& other) const;
	bool			operator!= ( const GVector<T> & other ) const;
	GVector<T>		operator+ ( const GVector<T> & other ) const;
	GVector<T> &	operator+= ( const GVector<T> & other );
	GVector<T> &	operator+= ( const T & value );
	GVector<T> &	operator<< ( const GVector<T> & other );
	GVector<T> &	operator<< ( const T & value );
	//return reference to element at position indx
	T&				operator[](uint indx);
	const T&		operator[](uint indx) const;

	//static members

	static GVector<T> fromSTDVector(const std::vector<T>& v);
protected:
	std::vector<T> _v;
};

template<class T> GVector<T>::GVector(uint size) : _v(size)								{}
template<class T> GVector<T>::GVector(uint size, const T& value) : _v(size,value)		{}
template<class T> GVector<T>::GVector(const GVector& other) : _v(other.toStdVector())	{}

template<class T> inline const T& GVector<T>::at(uint i) const							{return _v.at(i);}
template<class T> inline int GVector<T>::capacity() const								{return _v.capacity();}
template<class T> inline void GVector<T>::clear()										{_v.clear();}
template<class T> inline bool GVector<T>::contains(const T& value) const
{
	for (uint i=0; i<_v.size(); i++)
	{
		if(_v.at(i)==value)
			return true;
	}
	return false;
}
template<class T> inline int GVector<T>::count(const T& value) const
{
	int n=0;
	for(uint i=0; i<_v.size(); i++)
	{
		if(_v.at(i)==value)
			n++
	}
	return n;
}
template<class T> inline T* GVector<T>::data() {return _v.data();}
template<class T> inline GVector<T>& GVector<T>::fill(const T& value, int size)
{
	if(size<0)
		size = _v.size();
	_v.assign(size,value);
	return *this;
}
template<class T> inline T& GVector<T>::first() {return _v.front();}
template<class T> inline int GVector<T>::indexOf(const T& value, uint from) const
{
	if(from>=_v.size())
		return -1;
	for(int i=from; i<_v.size(); i++)
	{
		if(_v.at(i) == value)
			return i;
	}
	return -1;
}
template<class T> inline void GVector<T>::insert(uint index, const T& value)	{_v.insert(_v.begin()+index,value);}
template<class T> inline void GVector<T>::insert(uint index, uint number, const T& value)
{
	_v.reserve(_v.size()+number);
	_v.insert(_v.begin()+index,number,value);
}
template<class T> inline bool GVector<T>::isEmpty() const						{return _v.empty();}
template<class T> inline T& GVector<T>::last()									{return _v.back();}
template<class T> inline void GVector<T>::prepend(const T& value)				{_v.insert(_v.begin(),value);}
template<class T> inline void GVector<T>::remove(uint atIndx)					{_v.erase(_v.begin()+atIndx);}
template<class T> inline void GVector<T>::remove(uint atIndx, uint number)		{_v.erase(_v.begin()+atIndx,_v.begin()+atIndx+number);}
template<class T> inline void GVector<T>::replace(uint atIndx, const T& value)	{_v[atIndx]=value;}
template<class T> inline void GVector<T>::reserve(uint n)						{_v.reserve(n);}
template<class T> inline void GVector<T>::resize(uint newSize)					{_v.resize(newSize);}
template<class T> inline uint GVector<T>::size() const							{return _v.size();}
template<class T> inline void GVector<T>::squeeze()								{_v.shrink_to_fit();}
template<class T> inline void GVector<T>::swap(GVector<T>& other)				{_v.swap(other._v);}
template<class T> inline std::vector<T> GVector<T>::toStdVector () const		{return _v;}
template<class T> inline T GVector<T>::value(uint atIndx) const
{
	if(atIndx<_v.size())
		return _v.at(atIndx);
	return T();
}
template<class T> inline T GVector<T>::value(uint atIndx,const T& defaultValue) const
{
	if(atIndx<_v.size())
		return _v.at(atIndx);
	return defaultValue;
}

template<class T> inline GVector<T>& GVector<T>::operator=(const GVector<T>& other)
{
	if(*this==other)
		return *this;
	_v = other._v;
	return *this;
}
template<class T> inline bool GVector<T>::operator==(const GVector<T>& other) const {return _v==other._v;}
template<class T> inline bool GVector<T>::operator!=(const GVector<T>& other) const {return _v!=other._v;}
template<class T> inline GVector<T> GVector<T>::operator+(const GVector<T>& other) const
{
	GVector<T> res(_v);
	uint vSize = _v.size();
	uint newSize = vSize+other.size();
	res.reserve(newSize);
	for(uint i=vSize; i<newSize; i++)
	{
		res.append(other.at(i-vSize));
	}
	return res;
}
template<class T> inline GVector<T>& GVector<T>::operator+=(const GVector<T>& other) 
{
	uint vSize = _v.size();
	uint newSize = vSize+other.size();
	reserve(newSize);
	for(uint i=vSize; i<newSize; i++)
	{
		append(other.at(i-vSize));
	}
	return *this;
}
template<class T> inline GVector<T>& GVector<T>::operator+=(const T& value)				{append(value); return *this;}
template<class T> inline GVector<T>& GVector<T>::operator<<(const GVector<T>& other)	{return this->operator+=(other);}
template<class T> inline GVector<T>& GVector<T>::operator<<(const T& value)				{return this->operator+=(T);}
template<class T> inline T&			 GVector<T>::operator[](uint indx)					{return _v[indx];}
template<class T> inline const T&	 GVector<T>::operator[](uint indx) const			{return _v[indx];}

	//static members

template<class T> inline GVector<T> GVector<T>::fromSTDVector(const std::vector<T>& v) {GVector<T> x; x._v=v; return x;}

#endif // GVECTOR_H 
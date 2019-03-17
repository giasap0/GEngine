#pragma once
#ifndef GHEAP_H
#define GHEAP_H 

#include "GVector.h"
//** I do not use index 0 **//

//---------------------------------------------------------------------------
//class GHeap : to use as a priority queue (built like a binary ordered Tree)
//---------------------------------------------------------------------------
template<class T> class GHeap
{
public:
	//return<0 (left<right), return>0(left>right), return0(left==right)
	explicit GHeap( int(*p_compare)(const T&, const T&) )		:_v(1), _compare(p_compare)		{}
	GHeap( const GHeap& other )									: _v(other._v), _compare(other._compare)			{}
	virtual ~GHeap()											{_compare=nullptr;}

	inline GHeap& operator=(const GHeap& o)						{_v = o._v; _compare=o._compare; return *this;}
	inline uint size() const									{return _v.size()-1;}
	inline T& first()											{return _v[1];}
	inline const T& first() const								{return _v[1];}
	inline bool isEmpty() const									{if(_v.size()>1) return false; return true;}
	//clear elements, not function pointer
	inline void clear()											{_v.resize(1);}
	//clear elements and change function pointer
	inline void clear( int(*p_compare)(const T&, const T&) )	{_v.resize(1); _compare=p_compare;}

	//insert into queue
	void enqueue(const T& data);
	//removing the root node of the heap
	void dequeue();

	inline bool			operator==(const GHeap<T>& o) const		{return (_v==o._v && _compare==o._compare);}
	inline bool			operator!=(const GHeap<T>& o) const		{return !this->operator==(o);}
	inline GHeap<T>&	operator+=(const T& x )					{enqueue(x); return *this;}

private:
	void walkUp(int i);
	void walkDown(int i);

	GVector<T> _v;
	//return<0 (left<right), return>0(left>right), return0(left==right)
	int (*_compare)(const T&, const T&);
};

template<class T> inline void GHeap<T>::enqueue(const T& data)
{
	_v.append(data);
	walkUp( static_cast<int>(_v.size())-1 );
}
template<class T> inline void GHeap<T>::dequeue()
{
	int count = static_cast<int>( _v.size()-1 );
	if( count >= 1)
	{
		_v[1] = _v[count];		//root==last
		walkDown( 1 );			//re-order
		_v.remove( count);		//remove last element
		--count;
	}
}

template<class T> inline void GHeap<T>::walkDown(int i)
{
	int parent = i;
	int child = i*2;
	T temp = _v[parent];
	int count = _v.size()-1;
	while( child < count )
	{
		if( child < count-1 )
		{
			if( _compare( _v[child], _v[child+1] ) < 0)
				++child;
		}
		if( _compare(temp, _v[child]) < 0)
		{
			_v[parent] = _v[child];
			parent= child;
			child*=2;
		}
		else
			break;
	}
	_v[parent] = temp;
}

template<class T> inline void GHeap<T>::walkUp(int i)
{
	int parent = i/2;
	int child = i;
	T temp = _v[child];
	while(parent>0)
	{
		if( _compare( temp, _v[parent]) > 0)
		{
			_v[child] = _v[parent];
			child = parent;
			parent /=2;
		}
		else
			break;
	}//EO while (parent valid)
	_v[child] = temp;
}


#endif		//EO GHEAP_H

#pragma once
#ifndef GSTACK_H
#define GSTACK_H

#include "GLinkedList.h"

//---------------------------------------------------------------------------------------------
//class GStack: represent a stack (LIFO): you push item on top, and pop the last item from top
//---------------------------------------------------------------------------------------------
template<class T> class GStack
{
public:
	GStack(void)	:_list()	{}
	~GStack(void)				{}

	//stack operations

	inline void push(const T& item)	{_list.append(item);}
	inline void pop()				{_list.removeLast();}

	//access
	inline const T& top() const		{return _list.last();}
	inline		 T&	top()			{return _list.last();}

	inline bool isEmpty() const		{return _list.isEmpty();}
	inline int size() const			{return _list.size(); }

private:
	GLinkedList<T> _list;
};	//EO class GStack


#endif	//EO GSTACK_H

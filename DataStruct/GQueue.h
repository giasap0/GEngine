#pragma once
#ifndef GQUEUE_H
#define GQUEUE_H

#include "GLinkedList.h"

//--------------------------------------------------------------------------------------------------
//class GQueue: represent a stack (FIFO): you enqueue item on top, and dequeue the first item entered
//--------------------------------------------------------------------------------------------------
template<class T> class GQueue
{
public:
	GQueue(void)	:_list()	{}
	~GQueue(void)				{}

	//queue operations

	inline void enqueue(const T& item)				{_list.append(item);}
	inline void dequeue()							{_list.removeFirst();}

	//access
	inline const T& first() const					{return _list.first();}
	inline		 T& first()							{return _list.first();}

	inline GListIterator<T> begin()					{return _list.begin();}
	inline GList_ConstIterator<T> const_begin()		{return _list.const_begin();}

	inline bool isEmpty() const						{return _list.isEmpty();}
	inline int size() const							{return _list.size(); }

private:
	GLinkedList<T> _list;
};	//EO class GStack


#endif	//EO GSTACK_H

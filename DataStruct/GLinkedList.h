#pragma once
#ifndef GLINKEDLIST_H
#define GLINKEDLIST_H

template<class T> class GListIterator;
template<class T> class GList_ConstIterator;
template<class T> class GLinkedList;

namespace GListParts
{
	template<class T> class GListNode
	{
		friend class GLinkedList<T>;
		friend class GListIterator<T>;
		friend class GList_ConstIterator<T>;
	public:
		GListNode()		:_pNext(nullptr)		{}
		virtual ~GListNode()					{_pNext= nullptr;}
		inline void insertAfter(const T& data)
		{
			GListNode<T>* newNode = new GListNode<T>();
			newNode->_data = data;
			newNode->_pNext = _pNext;
			_pNext = newNode;
		}
		inline T& data()						{return _data;}
		inline const T& const_data() const		{return _data;}
		inline void setData(const T& newValue)	{_data=newValue;}
		inline GListNode<T>* next()				{return _pNext;}

	private:
		T _data;
		GListNode<T>* _pNext;
	};
};	//EO namespace GListParts

//-----------------------------------------------------------------------
//class GLinkedList : a singly linked list. Use GListIterator to iterate
//-----------------------------------------------------------------------
template<class T> class GLinkedList
{
	friend class GListIterator<T>;
public:
	GLinkedList()				:_head(nullptr), _tail(nullptr)	{_count=0;}
	GLinkedList(const GLinkedList& other);
	virtual ~GLinkedList();

	void append(const T& data);
	//it cause a copy of other
	void append(const GLinkedList<T>& other);
	void prepend(const T& data);
	//inserts an item AFTER the current iterator (append if iterator is invalid)
	void insert( const GListIterator<T>& iterator, const T& value);
	//iterator will be moved forward to the next node (to be valid again)
	void remove(GListIterator<T>& iterator);
	void removeFirst();
	void removeLast();
	void clear();
	void replace(const GListIterator<T>& iterator, const T& newValue);

	inline bool isEmpty() const						{if(_head==nullptr) return true; return false;}
	inline int size() const							{return _count;}

	GListIterator<T> begin();
	GList_ConstIterator<T> const_begin() const;
	GListIterator<T> end();
	GList_ConstIterator<T> const_end() const;

	inline T& first()								{return _head->data();}
	inline const T& first() const					{return _head->const_data();}
	inline T& last()								{return _tail->data();}
	inline const T& last() const					{return _tail->const_data();}

	GLinkedList<T>& operator =(const GLinkedList<T>& other);
	GLinkedList<T> operator+(const GLinkedList<T>& other) const		{GLinkedList<T> L(*this); L.append(other); return L;}
	inline GLinkedList<T> operator+(const T& appendMe) const		{GLinkedList<T> L(*this); L.append(appendMe); return L;}
	inline GLinkedList<T>& operator+=(const GLinkedList<T>& other)	{append(other); return *this;}
	inline GLinkedList<T>& operator+=(const T& appendMe)			{append(appendMe); return *this;}

private:
	GListParts::GListNode<T>* _head;		//first element
	GListParts::GListNode<T>* _tail;		//last element
	int _count;

};//EO class GLinkedList

template<class T> class GListIterator
{
	friend class GLinkedList<T>;
public:
	GListIterator( GLinkedList<T>* pList = nullptr, GListParts::GListNode<T>* pNode = nullptr)	: _pList(pList), _pNode(pNode)	{}
	GListIterator(const GListIterator<T>& it)	{_pList=it._pList; _pNode=it._pNode;}
	~GListIterator()							{_pNode= nullptr; _pList= nullptr;}

	inline T& data()							{return _pNode->data();}
	inline bool isValid() const					{return (_pNode!= nullptr);}

	inline bool operator==(const GListIterator<T>& it)
	{
		if(_pList != it._pList)
			return false;
		if(_pNode == it._pNode)
			return true;
		return false;
	}
	inline bool operator!=(const GListIterator<T>& it)		{return !this->operator==(it);}
	//prefix ++ returns a reference
	inline GListIterator<T>& operator++()
	{
		if(_pNode != nullptr)
			_pNode=_pNode->next();
		return *this;
	}
	// postfix ++ returns a value
	inline GListIterator<T> operator++(int)
	{
		GListIterator<T> result(*this);
		this->operator++();
		return result;	//return the old value
	}
	inline void gotoBegin()				{if(_pList != nullptr) _pNode= _pList->_head;}
	inline void gotoEnd()				{if(_pList != nullptr) _pNode = _pList->_tail;}
	inline void clear()					{_pNode= nullptr; _pList = nullptr;}

private:
	GListParts::GListNode<T>* _pNode;
	GLinkedList<T>* _pList;
};//EO class GListIterator

//------------------------------------------------------------------------------------------------------------
//class GList_ConstIterator : with this iterator you gain access to list members and you can't modify them
//------------------------------------------------------------------------------------------------------------
template<class T> class GList_ConstIterator
{
	friend class GLinkedList<T>;
public:
	GList_ConstIterator(const GLinkedList<T>* pList = nullptr, const GListParts::GListNode<T>* pNode = nullptr)	: _pList(pList), _pNode(pNode)	{}
	GList_ConstIterator(const GList_ConstIterator& it)														{_pList=it._pList; _pNode=it._pNode;}
	~GList_ConstIterator()																					{_pNode= nullptr; _pList= nullptr;}

	inline const T& data() const					{return _pNode->const_data();}
	inline bool isValid() const						{return (_pNode!= nullptr);}
	inline bool operator==(const GList_ConstIterator<T>& it)
	{
		if(_pList != it._pList)
			return false;
		if(_pNode == it._pNode)
			return true;
		return false;
	}
	inline bool operator!=(const GList_ConstIterator<T>& it)		{return !this->operator==(it);}
	//prefix ++ returns a reference
	inline GList_ConstIterator<T>& operator++()
	{
		if(_pNode != nullptr)
		{
			_pNode = _pNode->_pNext;
		}
		return *this;
	}
	// postfix ++ returns a value
	inline GList_ConstIterator<T> operator++(int)
	{
		GListIterator<T> result(*this);
		this->operator++();
		return result;	//return the old value
	}
	inline void gotoBegin()				{if(_pList != nullptr) _pNode= _pList->_head;}
	inline void gotoEnd()				{if(_pList != nullptr) _pNode = _pList->_tail;}
	inline void clear()					{_pList = nullptr; _pNode = nullptr;}

private:
	const GListParts::GListNode<T>* _pNode;
	const GLinkedList<T>* _pList;
};//EO class GList_ConstIterator


template<class T> inline GLinkedList<T>::~GLinkedList()
{
	using GListParts::GListNode;
	GListNode<T>* itr = _head;
	GListNode<T>* next = nullptr;
	while( itr != nullptr )
	{
		next = itr->next();
		delete itr;
		itr = next;
	}
}

template<class T> inline GLinkedList<T>::GLinkedList(const GLinkedList<T>& L)
{
	using GListParts::GListNode;
	_head = _tail = nullptr;
	_count = 0;
	if(L.isEmpty() )
	{
		return;
	}
	GListNode<T>* ptr = L._head;
	while( ptr != L._tail )
	{
		append( ptr->const_data() );
		ptr = ptr->_pNext;
	}
	append( L._tail->const_data() );
}

template<class T> inline void GLinkedList<T>::append(const T& data)
{
	if(_head == nullptr)
	{
		_head = _tail= new GListParts::GListNode<T>;
		_head->data() = data;
	}
	else
	{
		_tail->insertAfter(data);
		_tail = _tail->next();
	}
	_count++;
}
template<class T> inline void GLinkedList<T>::append(const GLinkedList<T>& L)
{
	using GListParts::GListNode;
	if(_head == nullptr)
		this->operator=(L);
	else
	{
		GListNode<T>* ptr = L._head;
		while( ptr != L._tail )
		{
			append( ptr->const_data() );
			ptr = ptr->_pNext;
		}
		append( L._tail->const_data() );
	}
}

template<class T> inline void GLinkedList<T>::prepend(const T& data)
{
	using GListParts::GListNode;
	GListNode<T>* newNode = new GListNode<T>;
	newNode->data() = data;
	newNode->_pNext = _head;
	_head = newNode;
	if(_tail == nullptr)
		_tail = _head;
	_count++;
}

template<class T> inline void GLinkedList<T>::removeFirst()
{
	using GListParts::GListNode;
	GListNode<T>* node = nullptr;
	if( _head != nullptr )
	{
		node = _head->next();
		delete _head;
		_head = node;
		if(_head == nullptr)		//was 1 element list
			_tail= nullptr;
		_count--;
	}
}

template<class T> inline void GLinkedList<T>::removeLast()
{
	using GListParts::GListNode;
	GListNode<T>* node = _head;
	if( _head != nullptr)				//list not empty
	{
		if(_head == _tail)				//only 1 element
		{
			delete _head;
			_head = _tail = nullptr;
		}
		else
		{
			while(node->next() != _tail)	//go to one before tail
			{
				node = node->next();
			}
			_tail= node;					//reassign tail
			delete node->next();			//delete last
			node->next() = nullptr;
		}
		_count--;
	}//EO (list not empty)
}

template<class T> inline void GLinkedList<T>::clear()
{
	GListParts::GListNode<T>* itr = _head;
	GListParts::GListNode<T>* next = nullptr;
	while( itr != nullptr )
	{
		next = itr->next();
		delete itr;
		itr = next;
	}
	_head = _tail = nullptr;
	_count=0;
}

template<class T> inline GListIterator<T> GLinkedList<T>::begin()
{
	return GListIterator<T> (this,_head);
}

template<class T> inline GListIterator<T> GLinkedList<T>::end()
{
	return GListIterator<T> (this,_tail);
}

template<class T> inline GList_ConstIterator<T> GLinkedList<T>::const_begin() const
{
	return GList_ConstIterator<T>(this,_head);
}

template<class T> inline GList_ConstIterator<T> GLinkedList<T>::const_end() const
{
	return GList_ConstIterator<T>(this,_tail);
}

template<class T> inline void GLinkedList<T>::insert( const GListIterator<T>& itr, const T& data)
{
	if(itr._pList != this)						//iterator doesn't belong to this list
		return;
	if( itr._pNode != nullptr )					//iterator is valid
	{
		itr._pNode->insertAfter(data);
		if( itr._pNode == _tail )				//if itr is the tail, update tail
		{
			_tail = itr._pNode->next();
		}
		_count++;
	}
	else										//iterator is invalid
	{
		append(data);
	}
}

template<class T> inline void GLinkedList<T>::remove(GListIterator<T>& itr)
{
	using GListParts::GListNode;
	GListNode<T>* node = _head;
	if(itr._pList != this)						//iterator doesn't belong to this list
		return;
	if(itr._pNode == nullptr )					//iterator is invalid
		return;
	if(itr._pNode == _head )					//iterator is head
	{
		++itr;
		removeFirst();
	}
	else
	{
		while(node->next() != itr._pNode )		//go to node before the node to delete
			node = node->next();
		++itr;									//move iterator to the node next to the one to delete
		if( node->next() == _tail)
		{
			_tail= node;						//if is the case update tail
		}
		delete node->_pNext;
		node->_pNext = itr._pNode;				//re-link the list
		_count--;
	}
}

template<class T> inline void GLinkedList<T>::replace(const GListIterator<T>& itr, const T& newValue)
{
	if(itr._pList != this)						//check iterator validity
		return;
	if(itr._pNode == nullptr)
		return;
	itr._pNode->data() = newValue;
}

template<class T> inline GLinkedList<T>& GLinkedList<T>::operator=(const GLinkedList<T>& L)
{
	using GListParts::GListNode;
	if( this == &L)
		return *this;
	clear();
	if(L.isEmpty() )
	{
		return *this;
	}
	GListNode<T>* ptr = L._head;
	while( ptr != L._tail )
	{
		append( ptr->const_data() );
		ptr = ptr->_pNext;
	}
	append( L._tail->const_data() );
}

#endif	//EO GLINKEDLIST_H
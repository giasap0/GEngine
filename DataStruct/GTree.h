#pragma once
#ifndef GTREE_H
#define GTREE_H 

#include "GLinkedList.h"
template<class T> class GTreeIterator;

//----------------------------------------------------------------------------------------
//Class GTree : every node stores one <dataType> , have one parent and a list of children
//----------------------------------------------------------------------------------------
template<class T> class GTree
{
	friend class GTreeIterator<T>;
	typedef GTree<T> Node;
public:
	GTree(void)							: _parent(nullptr)						{}
	explicit GTree(const T& nodeValue)	: _data(nodeValue), _parent(nullptr)	{}
	virtual ~GTree(void)														{clearChildren();}

	//delete all children from this node
	inline void clearChildren()
	{
		GListIterator<Node*> itr = _children.begin();
		Node* node = nullptr;
		while(itr.isValid() )
		{
			node = itr.data();
			_children.remove(itr);				//this makes ++iterator
			delete node;						//in this line: recursion
		}
	}

	//return total number of nodes
	inline int size()	const
	{
		int c = 1;
		GList_ConstIterator<Node*> itr = _children.const_begin();
		while (itr.isValid() )
		{
			c += itr.data()->size();
			++itr;
		}
		return c;
	}
	inline const T& value() const		{return _data;}
	inline void setValue(const T& v)	{_data=v;}
	inline bool haveChildren() const	{return !_children.isEmpty();}

	//0==only root, 1==root children are leafs, and so on
	int depth() const;
	//current node is processed before the children (root->first child->..-->leaves,root->second child.. and so on)
	inline static void iteratePreorder( Node* p_node, void(*ptr_function)(GTree<T>*) )
	{
		if(p_node==nullptr)
			return;
		ptr_function( p_node );
		GListIterator< Node* > itr = p_node->_children.begin();
		while ( itr.isValid() )
		{
			GTree<T>::iteratePreorder( itr.data(), ptr_function );
			++itr;
		}
	}

	//process from leaves to root ( root == last element processed)
	inline static void iteratePostorder( Node* p_node, void(*ptr_function)(GTree<T>*) )
	{
		if(p_node==nullptr)
			return;
		GListIterator< Node* > itr = p_node->_children.begin();
		while ( itr.isValid() )
		{
			GTree<T>::iteratePostorder( itr.data(), ptr_function );
			++itr;
		}
		ptr_function( p_node );
	}

private:
	static int nodeDepth(GTree<T>* t);

	GTree(const GTree<T>&);										//TODO: copy
	GTree<T>& operator=(const GTree<T>&);

	T _data;
	Node* _parent;
	GLinkedList<Node*> _children;
};	//EO class GTree

//------------------------------------------------------
// TreeIterator<int> itr( node );
//or directly
// itr = node;
//------------------------------------------------------
template<class T> class GTreeIterator
{
	typedef GTree<T> Node;
public:
	GTreeIterator( Node* ptr = nullptr)				{*this = ptr;}

	inline GTreeIterator& operator= (GTree<T>* ptr)				{_pNode=ptr; resetIterator(); return *this;}
	inline GTreeIterator& operator=(const GTreeIterator& it)	{_pNode=it._pNode; _child_itr=it._child_itr; return *this;}
	
	inline void resetIterator()						{if(_pNode != nullptr) _child_itr = _pNode->_children.begin();	else _child_itr.clear();}
	inline void clear()								{_pNode= nullptr; _child_itr.clear();}
	//vertical functions
	
	void moveToRoot();
	//true if parent exists
	inline bool up()
	{
		if(_pNode != nullptr)
		{
			_pNode = _pNode->_parent;
		}
		resetIterator();
		if(_pNode==nullptr)
			return false;
		else
			return true;
	}
	//move to THE CURRENT child (true if child exists)
	inline bool down()
	{
		if(_child_itr.isValid())
		{
			_pNode = _child_itr.data();
			resetIterator();
			return true;
		}
		else
			return false;
	}
	inline bool haveChildren() const			{if(_pNode == nullptr || _pNode->_children.isEmpty() ) return false; return true;}

	//horizontal functions

	//true == i moved child iterator and is valid
	inline bool childRight()
	{
		if(_pNode == nullptr || !_child_itr.isValid() )
			return false;
		++_child_itr; 
		if(_child_itr.isValid()) 
			return true; 
		else
			return false;
	}
	void childLeft();
	inline void childStart()										{resetIterator();}
	inline void childEnd()											{if(_pNode!= nullptr) _child_itr = _pNode->_children.end();}

	//other functions
	
	inline void appendChild(const T& childValue)
	{
		if( _pNode == nullptr)
			return;
		Node* child = new Node(childValue);
		child->_parent = _pNode;
		_pNode->_children.append(child);
		if(!_child_itr.isValid())
			_child_itr = _pNode->_children.begin();
	}
	inline void prependChild(const T& childValue)	
	{
		if( _pNode == nullptr)
			return;
		Node* child = new Node(childValue);
		child->_parent = _pNode;
		_pNode->_children.prepend(child);
		if(!_child_itr.isValid())
			_child_itr = _pNode->_children.begin();
	}
	inline void insertChildAfter(const T& childValue)
	{
		if( _pNode == nullptr)
			return;
		Node* child = new Node(childValue);
		child->_parent = _pNode;
		_pNode->_children.insert(_child_itr,child);
	}
	inline void removeChild()
	{
		if( _pNode == nullptr)
			return;
		Node* node = _child_itr.data();
		_pNode->_children.remove(_child_itr);
		delete node;
	}
	inline void insertChildBefore(const T& childValue)
	{
		if( _pNode == nullptr)
			return;
		GListIterator<T> it = _pNode->_children.begin();
		while(it != _child_itr)
			++it;
		Node* child = new Node(childValue);
		child->_parent = _pNode;
		_pNode->_children.insert(it,child);
	}

	inline bool isValid() const									{return (_pNode!= nullptr);}
	inline bool isChildValid() const							{return _child_itr.isValid(); }

	//value in this node
	inline T& value()											{return _pNode->_data;}
	inline const T& const_value() const							{return _pNode->_data;}
	inline Node* node()											{return _pNode;}

	//return a pointer to a Tree with root==child i'm pointing
	inline Node* child()										{return _child_itr.data();}
	inline T& childValue()										{return _child_itr.data()->_data;}
	//return number of children
	inline int childrenNumber() const							{return _pNode->_children.size();}

private:
	Node* _pNode;
	GListIterator< Node* > _child_itr;
};//EO classGTreeIterator

template<class T> void GTreeIterator<T>::moveToRoot()
{
	if(_pNode == nullptr)
	{
		_child_itr.clear();
		return;
	}
	while (_pNode->_parent != nullptr)
	{
		_pNode = _pNode->_parent;
	}
	_child_itr = _pNode->_children.begin();
}

template<class T> void GTreeIterator<T>::childLeft()
{
	if(_pNode == nullptr || !_child_itr.isValid() )
		return;
	auto it = _pNode->_children.begin();
	while( it != _child_itr )
		++it;
	_child_itr = it;
}

template<class T> int GTree<T>::nodeDepth(GTree<T>* t)
{
	int d=0;
	GTree<T>* ptr = t->_parent;
	do
	{
		++d;
		ptr = ptr->_parent;
	}
	while( ptr != nullptr );
	return d;
}


template<class T> int GTree<T>::depth() const
{
	int d = 0;
	GList_ConstIterator<Node*> itr = _children.const_begin();
	while(itr.isValid() )
	{
		int dep =0;
		if( itr.data()->haveChildren() )
			dep= itr.data()->depth();
		else
			dep = GTree<T>::nodeDepth( itr.data() );				//it is a leaf
		++itr;
		if( dep>d)
			d=dep;
	}
	return d;
}

#endif	//EO GTREE_H
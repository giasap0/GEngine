#pragma once
#ifndef GBTREE_H
#define GBTREE_H 

template<class T> class GBTreeIterator;

//-------------------------------------------------
//Class GBTree : represent a linked binary tree
//-------------------------------------------------
template<class T> class GBTree
{
	friend class GBTreeIterator<T>;
	typedef GBTree<T> Node;
public:
	GBTree()				: _pParent(nullptr),_pLeftCh(nullptr),_pRightCh(nullptr)				{}
	GBTree(const T& val)	: _data(val), _pParent(nullptr),_pLeftCh(nullptr),_pRightCh(nullptr)	{}
	virtual ~GBTree()		{clearChildren();}

	//delete all children from this node
	inline void clearChildren()
	{
		if( _pLeftCh != nullptr )
		{
			delete _pLeftCh;
			_pLeftCh = nullptr;
		}
		if( _pRightCh != nullptr )
		{
			delete _pRightCh;
			_pRightCh = nullptr;
		}
	}
	//return total number of nodes
	inline int size()	const
	{
		int c = 1;
		if( _pLeftCh!= nullptr)
		{
			c+= _pLeftCh->size();
		}
		if( _pRightCh != nullptr)
		{
			c+= _pRightCh->size();
		}
		return c;
	}
	inline const T& value() const		{return _data;}
	inline void setValue(const T& v)	{_data=v;}
	inline bool haveChildren() const	{if (_pLeftCh== nullptr && _pRightCh==nullptr) return false; return true;}

	//0==only root, 1==root children are leafs, and so on
	int depth() const;

	inline static void iteratePreorder( Node* p_node, void(*ptr_function)(GBTree<T>*) )
	{
		if(p_node==nullptr)
			return;
		ptr_function( p_node );
		if( p_node->_pLeftCh != nullptr)
			GBTree<T>::iteratePreorder(p_node->_pLeftCh,ptr_function );
		if( p_node->_pRightCh != nullptr)
			GBTree<T>::iteratePreorder(p_node->_pRightCh ,ptr_function );
	}

	//process from leaves to root ( root == last element processed)
	inline static void iteratePostorder( Node* p_node, void(*ptr_function)(GBTree<T>*) )
	{
		if(p_node==nullptr)
			return;
		if( p_node->_pLeftCh != nullptr)
			GBTree<T>::iteratePostorder(p_node->_pLeftCh,ptr_function );
		if( p_node->_pRightCh != nullptr)
			GBTree<T>::iteratePostorder(p_node->_pRightCh ,ptr_function );
		ptr_function( p_node );
	}
	inline static void iterateInOrder( Node* p_node,  void(*ptr_function)(GBTree<T>*) )
	{
		if(p_node==nullptr)
			return;
		if( p_node->_pLeftCh != nullptr)
			GBTree<T>::iterateInOrder(p_node->_pLeftCh, ptr_function);
		ptr_function( p_node );
		if( p_node->_pRightCh != nullptr)
			GBTree<T>::iterateInOrder(p_node->_pRightCh ,ptr_function );
	}


private:
	static int nodeDepth(GBTree<T>* t);
	int nodeDepth() const;

	GBTree(const GBTree&);
	GBTree& operator=(const GBTree&);

	T _data;
	Node* _pParent;
	Node* _pLeftCh;
	Node* _pRightCh;

};//EO class GBTree

//-------------------------------------------------
//Class GBTreeIterator : to iterate a binary tree
//-------------------------------------------------
template<class T> class GBTreeIterator
{
	typedef GBTree<T> Node;
public:
	GBTreeIterator( Node* ptr = nullptr)				{*this = ptr;}
	~GBTreeIterator()									{_pNode=nullptr;}

	inline GBTreeIterator& operator= (GBTree<T>* ptr)				{_pNode=ptr; return *this;}
	inline GBTreeIterator& operator=(const GBTreeIterator& it)		{_pNode=it._pNode; return *this;}
	
	inline void clear()												{_pNode= nullptr;}

	//vertical functions
	
	inline void moveToRoot()
	{
		if( _pNode != nullptr )
		{
			while ( _pNode->_pParent != nullptr )
				_pNode= _pNode->_pParent;
		}
	}

	//true if parent exists
	inline bool up()
	{
		if(_pNode != nullptr)
		{
			if( _pNode->_parent == nullptr)
				return false;
			_pNode = _pNode->_parent;
		}
		return true;
	}

	//move to the left child if exists
	inline bool leftChild()
	{
		if(_pNode->_pLeftCh != nullptr)
		{
			_pNode = _pNode->_pLeftCh;
			return true;
		}
		else
			return false;
	}
	//move to the right child if exists
	inline bool rightChild()
	{
		if(_pNode->_pRightCh != nullptr)
		{
			_pNode = _pNode->_pRightCh;
			return true;
		}
		else
			return false;
	}
	inline bool haveChildren() const
	{
		if(_pNode == nullptr)
			return false;
		if (_pNode->_pLeftCh == nullptr && _pNode->_pRightCh == nullptr)
			return false;
		return true;
	}

	//other functions
	
	//return false if node have 2 children or iterator is invalid
	//if left==null append to left, otherwise append to right
	inline bool appendChild(const T& childValue)
	{
		if( _pNode == nullptr)
			return false;
		if( _pNode->_pLeftCh != nullptr && _pNode->_pRightCh != nullptr)
			return false;
		Node* child = new Node(childValue);
		child->_pParent = _pNode;
		if( _pNode->_pLeftCh == nullptr)
		{
			_pNode->_pLeftCh = child;
			return true;
		}
		else
		{
			_pNode->_pRightCh = child;
			return true;
		}
	}
	inline bool appendLeftChild(const T& childValue)
	{
		if( _pNode == nullptr)
			return false;
		if( _pNode->_pLeftCh != nullptr)
			return false;
		Node* child = new Node(childValue);
		child->_pParent = _pNode;
		_pNode->_pLeftCh = child;
		return true;
	}
	inline bool appendRightChild(const T& childValue)
	{
		if( _pNode == nullptr)
			return false;
		if( _pNode->_pRightCh != nullptr)
			return false;
		Node* child = new Node(childValue);
		child->_pParent = _pNode;
		_pNode->_pRightCh = child;
		return true;
	}

	inline bool removeLeftChild()
	{
		if( _pNode == nullptr)
			return false;
		if( _pNode->_pLeftCh == nullptr)
			return false;
		delete _pNode->_pLeftCh;
		_pNode->_pLeftCh = nullptr;
		return true;
	}
	inline bool removeRightChild()
	{
		if( _pNode == nullptr)
			return false;
		if( _pNode->_pRightCh == nullptr)
			return false;
		delete _pNode->_pRightCh;
		_pNode->_pRightCh = nullptr;
		return true;
	}

	inline bool isValid() const									{return (_pNode!= nullptr);}
	inline bool isLeftChildValid() const						{return (_pNode->_pLeftCh != nullptr); }
	inline bool isRightChildValid() const						{return (_pNode->_pRightCh != nullptr); }

	//value in this node
	inline T& value()											{return _pNode->_data;}
	inline const T& const_value() const							{return _pNode->_data;}
	inline Node* node()											{return _pNode;}

	//return a pointer to a Tree with root==left child
	inline Node* getLeftChild()									{return _pNode->_pLeftCh;}
	inline Node* getRightChild()								{return _pNode->_pRightCh;}
	//return number of children
	inline int childrenNumber() const
	{
		int c=0;
		if(_pNode->_pLeftCh!= nullptr)
			c+=1;
		if(_pNode->_pRightCh!= nullptr)
			c+=1;
		return c;
	}

private:
	Node* _pNode;
};//EO classGTreeIterator

template<class T> int GBTree<T>::nodeDepth(GBTree<T>* t)
{
	int d=0;
	GBTree<T>* ptr = nullptr;
	if (t == nullptr)
		return 0;
	ptr = t->_pParent;
	while( ptr != nullptr );
	{
		++d;
		ptr = ptr->_parent;
	}
	return d;
}

template<class T> int GBTree<T>::nodeDepth() const
{
	int d = 0;
	Node* ptr = _pParent;
	while (ptr != nullptr)
	{
		++d;
		ptr = ptr->_pParent;
	}
	return d;
}


template<class T> int GBTree<T>::depth() const
{
	int d =0;
	if(! haveChildren() )							//leaf
	{
		return nodeDepth();
	}
	else
	{
		int depL=0, depR=0;
		if( _pLeftCh!= nullptr)
		{
			depL = _pLeftCh->depth();
		}
		if(_pRightCh != nullptr)
		{
			depR = _pRightCh->depth();
		}
		if(depL>d)
			d=depL;
		if(depR>d)
			d=depR;
		return d;
	}
}

#endif	//EO GBTREE_H
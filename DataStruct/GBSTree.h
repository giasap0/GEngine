#pragma once
#ifndef GBSTREE_H
#define GBSTREE_H 

#include "GBTree.h"

//-------------------------------------------------
//Class GBTree : represent a Binary Search Tree
//-------------------------------------------------
template<class T> class GBSTree
{
	typedef GBTree<T> Node;
public:
	GBSTree( int(*p_compare)(T,T) )  : _pRoot(nullptr), _compare(p_compare)		{}
	virtual ~GBSTree()															{if(_pRoot!= nullptr) delete _pRoot; _pRoot= nullptr;}

	//left if _compare<0, else right (do not insert if data exists)
	inline void insert(const T& data)
	{
		GBTreeIterator<T> it = _pRoot;
		if( _pRoot== nullptr)
			_pRoot = new Node( data );
		else
		{
			while (it.isValid() )
			{
				int comp = _compare(data, it.value() );
				if (comp < 0)
				{
					if( !it.leftChild() )
					{
						it.appendLeftChild(data);
						it.clear();
					}
				}
				else if(comp==0)
					return;
				else
				{
					if( !it.rightChild() )
					{
						it.appendRightChild(data);
						it.clear();
					}
				}
			}//	EO while (it.isValid )
		}// EO else {root exists}
	}

	//return an iterator pointing to the node with value==data (return invalid it if doesn't exists)
	inline GBTreeIterator<T> find( const T& data)
	{
		GBTreeIterator<T> current = _pRoot;
		int temp;
		while( current.isValid() )
		{
			temp = _compare( data, current.value() );
			if ( temp == 0 )
				return current;
			else if ( temp<0 )
			{
				if (!current.leftChild() )
					current.clear();
			}
			else
			{
				if (current.rightChild() )
					current.clear();
			}
		}
		current.clear();
		return current;
	}

	inline int size() const		{return _pRoot->size();}
	inline int depth() const	{return _pRoot->depth();}			

private:
	GBSTree(const GBSTree&);
	GBSTree& operator=(const GBSTree&);

	//return<0 (left<right), return>0(left>right), return0(left==right)
	int (*_compare)(const T&, const T&);	
	Node* _pRoot;
};//EO class GBTree

#endif	//EO GBSTREE_H
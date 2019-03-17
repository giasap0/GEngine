#pragma once
#ifndef GGRAPH_H
#define GGRAPH_H

#include "GVector.h"
#include "GQueue.h"

//-------------------------------------------
//class GGraph: represent a vectorized graph
//-------------------------------------------
template<class T> class GGraph
{
private:
	class Node;
	class Arc
	{
		friend class GGraph<T>;
		friend class Node;
	public:
				Arc()				: _pointer(nullptr)	{}
		explicit Arc(Node* ptr)		: _pointer(ptr)		{}
		inline virtual ~Arc()		{_pointer=nullptr;}
		
		inline bool operator==(const Arc& a) const		{return (_pointer==a._pointer);}
		inline bool isValid() const						{return (_pointer!= nullptr);}
	private:
		//Weight _weight;
		Node* _pointer;
	};	//EO class Arc

	class Node
	{
		friend class GGraph<T>;
		//friend class Arc;
	public:
		Node()			:_arcs(0)						{_marked=false;}
		virtual ~Node()									{}

		inline T& data()								{return _data;}
		inline const T& data() const					{return _data;}

		inline bool operator==(const Node& n) const		{return (_data==n._data && _arcs==n._arcs);}
		//return false for nullPointers
		inline bool existsArcTo( Node* ptr) const		{if(ptr==nullptr)return false; for(uint i=0; i<_arcs.size(); i++){if(_arcs[i]._pointer == ptr) return true;} return false;}	
		//return nullptr if this node do not have arc to ptr
		inline Arc* getArc(Node* ptr)					{if(ptr==nullptr)return nullptr; for(uint i=0; i<_arcs.size(); i++) {if(_arcs[i]._pointer==ptr) return &_arcs[i];} return nullptr;}
		inline bool removeArc(Node* ptr)				{if(ptr==nullptr)return false; for(uint i=0; i<_arcs.size(); i++)	{if(_arcs[i]._pointer==ptr) {_arcs.remove(i); return true;}} return false;}
		inline bool addArc(Node* ptr)					{if (existsArcTo(ptr) || ptr==nullptr)	return false; Arc a(ptr); _arcs.append(a); return true;	}

	private:
		T _data;
		GVector<Arc> _arcs;
		bool _marked;
		//do not allow copy
		Node( const Node&);
		Node& operator=(const Node&);
	};	//EO class Node
	
public:
			GGraph()	:		_nodes(0)															{}
	virtual ~GGraph();

	//Warning : slow operation (==removing item in array)
	void removeNode(uint index);
	void removeNodes(uint fromIndex, uint number);
	inline void appendNode(const T& data);
	inline uint size() const																		{return _nodes.size();}
	inline bool isEmpty() const																		{return _nodes.isEmpty();}
	bool addArc(uint fromNode, uint toNode);
	bool removeArc(uint fromNode, uint toNode);
	bool existsArc(uint fromNode, uint toNode) const;
	// ArcWeight getArcWeight(uint fromNode, uint toNode) const;	//TODO
	
	void processDepthFirst(uint startNodeIndx, void(*functionPtr)(T& modifyData));
	void processBreadthFirst(uint startNodeIndx, void(*functionPtr)(T& modifyData));

private:
	void setNodesMarked(bool marked = true);
	void processDepthFirst(Node* startNode, void(*functionPtr)(T& modifyData));

	GVector<Node*> _nodes;
}; //EO Class GGraph

template<class T> inline GGraph<T>::~GGraph()
{
	if( _nodes.isEmpty() )
		return;
	for(uint i=0; i<_nodes.size(); i++){
		if(_nodes[i] != nullptr)
			{delete _nodes[i]; _nodes[i]=nullptr;}
	}
}

template<class T> inline void GGraph<T>::removeNode(uint index)
{
	if( _nodes[index] != nullptr )
	{
		delete _nodes[index];
		_nodes.remove(index);
	}
}

template<class T> inline void GGraph<T>::removeNodes(uint index, uint number)
{
	uint size = _nodes.size();
	for( uint i =0 ; i<size; i++)
	{
		if( _nodes[index] != nullptr )
		{
			delete _nodes[index];
		}
	}
	_nodes.remove(index,number);
}
template<class T> inline void GGraph<T>::appendNode(const T& data)
{
	Node* node = new Node;
	node->_data = data;
	_nodes.append( node );
}


template<class T>  inline bool GGraph<T>::addArc(uint fromNode, uint toNode)
{
	uint size = _nodes.size();
	if( fromNode == toNode)
		return false;
	if( fromNode>=size || toNode>=size)
		return false;
	if( (_nodes[fromNode] == nullptr) || (_nodes[toNode] == nullptr) )
		return false;
	return _nodes[fromNode]->addArc( _nodes[toNode] );
}

template<class T>  inline bool GGraph<T>::removeArc(uint fromNode, uint toNode)
{
	uint size = _nodes.size();
	if( fromNode == toNode)
		return false;
	if( fromNode>=size || toNode>=size)
		return false;
	if( (_nodes[fromNode] == nullptr) || (_nodes[toNode] == nullptr) )
		return false;
	return _nodes[fromNode]->removeArc( _nodes[toNode] );
}

template<class T>  inline bool GGraph<T>::existsArc(uint fromNode, uint toNode) const
{
	uint size = _nodes.size();
	if( fromNode == toNode)
		return false;
	if( fromNode>=size || toNode>=size)
		return false;
	if( (_nodes[fromNode] == nullptr) || (_nodes[toNode] == nullptr) )
		return false;
	return _nodes[fromNode]->existsArcTo( _nodes[toNode] );
}

template<class T> inline void GGraph<T>::setNodesMarked(bool marked)
{
	uint size = _nodes.size();
	if (size==0 )
		return;
	for( uint i=0; i<size; i++)
	{
		if( _nodes[i] != nullptr )
			_nodes[i]->_marked = marked;
	}
	return;
}

template<class T> inline void GGraph<T>::processDepthFirst(uint startNodeIndx, void(*functionPtr)(T&))
{
	if( _nodes.isEmpty() || startNodeIndx >= _nodes.size() )
		return;
	Node<T>* ptr = _nodes[startNodeIndx];
	if (ptr == nullptr)
		return;
	processDepthFirst(ptr,functionPtr);
	ptr = nullptr;
	setNodesMarked(false);
}

template<class T> inline void GGraph<T>::processDepthFirst(Node* startNode, void(*functionPtr)(T& modifyData))
{
	functionPtr( startNode->_data );
	startNode->_marked = true;

	for(uint i=0; i< startNode->_arcs.size(); i++)
	{
		if ( startNode->_arcs[i]._pointer->_marked == false)
			processDepthFirst(  startNode->_arcs[i]._pointer, functionPtr );
	}
}

template<class T> inline void GGraph<T>::processBreadthFirst(uint startNodeIndx, void(*functionPtr)(T&))
{
	if( _nodes.isEmpty() || startNodeIndx >= _nodes.size() )
		return;
	Node<T>* startNode = _nodes[startNodeIndx];
	if (startNode == nullptr)
		return;
	
	GQueue< Node<T>* > queue;		//FIFO
	queue_.enqueue( startNode );
	startNode->_marked = true;
	while( !queue.isEmpty() )
	{
		functionPtr( queue.first()->_data );
		for( uint itr=0; itr< queue.first()->_arcs.size(); itr++)
		{
			if( queue.first()->_arcs[itr]._pointer->_marked == false )
			{
				queue.first()->_arcs[itr]._pointer->_marked = true;
				queue.enqueue( queue.first()->_arcs[itr]._pointer );
			}
		}
		queue.dequeue();
	} //EO while(queue not empty )

	startNode = nullptr;
	setNodesMarked(false);
}

#endif	//EO GGRAPH_H

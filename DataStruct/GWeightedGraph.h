#pragma once
#ifndef GWEIGHTEDGRAPH_H
#define GWEIGHTEDGRAPH_H

#include "GVector.h"
#include "GQueue.h"


//--------------------------------------------------------------
//class GWeightedGraph: represent a vectorized graph with weighted Arcs
//--------------------------------------------------------------
template<class T, class Weight> class GWeightedGraph
{
private:
	class Node;
	class Arc
	{
		friend class GWeightedGraph<T, Weight>;
		friend class Node;
	public:
				Arc()							: _weight( Weight() ), _pointer(nullptr)	{}
		explicit Arc(Node* ptr, Weight w)		: _weight(w), _pointer(ptr)					{}
		inline virtual ~Arc()					{_pointer=nullptr;}
		
		inline Weight weight() const					{return _weight;}
		inline void setWeight(const Weight& w)			{_weight=w;}

		inline bool operator==(const Arc& a) const		{return (_pointer==a._pointer);}
		inline bool isValid() const						{return (_pointer!= nullptr);}

	private:
		Weight _weight;
		Node* _pointer;
	};	//EO class Arc

	class Node
	{
		friend class GWeightedGraph<T, Weight>;
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
		inline bool addArc(Node* ptr, Weight w)			{if (existsArcTo(ptr) || ptr==nullptr)	return false; Arc a(ptr,w); _arcs.append(a); return true;	}

	private:
		T _data;
		GVector<Arc> _arcs;
		bool _marked;
		//do not allow copy
		Node( const Node&);
		Node& operator=(const Node&);
	};	//EO class Node
	
public:
			GWeightedGraph()	:		_nodes(0)													{}
	virtual ~GWeightedGraph();

	//Warning : slow operation (==removing item in array)
	void removeNode(uint index);
	void removeNodes(uint fromIndex, uint number);
	void appendNode(const T& data);
	bool addArc(uint fromNode, uint toNode, Weight w);
	bool removeArc(uint fromNode, uint toNode);
	bool existsArc(uint fromNode, uint toNode) const;
	//Warning: first check if arc exists
	Weight arcWeight(uint fromNode, uint toNode) const;

	inline uint size() const																		{return _nodes.size();}
	inline bool isEmpty() const																		{return _nodes.isEmpty();}
	
	void processDepthFirst(uint startNodeIndx, void(*functionPtr)(T& modifyData));
	void processBreadthFirst(uint startNodeIndx, void(*functionPtr)(T& modifyData));

private:
	void setNodesMarked(bool marked = true);
	void processDepthFirst(Node* startNode, void(*functionPtr)(T& modifyData));

	GVector<Node*> _nodes;
}; //EO Class GWeightedGraph

template<class T, class Weight> inline GWeightedGraph<T , Weight>::~GWeightedGraph()
{
	if( _nodes.isEmpty() )
		return;
	for(uint i=0; i<_nodes.size(); i++){
		if(_nodes[i] != nullptr)
			{delete _nodes[i]; _nodes[i]=nullptr;}
	}
}

template<class T, class Weight> inline void GWeightedGraph<T , Weight>::removeNode(uint index)
{
	if( _nodes[index] != nullptr )
	{
		delete _nodes[index];
		_nodes.remove(index);
	}
}

template<class T, class Weight> inline void GWeightedGraph<T , Weight>::removeNodes(uint index, uint number)
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
template<class T, class Weight> inline void GWeightedGraph<T , Weight>::appendNode(const T& data)
{
	Node* node = new Node;
	node->_data = data;
	_nodes.append( node );
}


template<class T, class Weight>  inline bool GWeightedGraph<T , Weight>::addArc(uint fromNode, uint toNode, Weight w)
{
	uint size = _nodes.size();
	if( fromNode == toNode)
		return false;
	if( fromNode>=size || toNode>=size)
		return false;
	if( (_nodes[fromNode] == nullptr) || (_nodes[toNode] == nullptr) )
		return false;
	return _nodes[fromNode]->addArc( _nodes[toNode],w );
}

template<class T, class Weight>  inline bool GWeightedGraph<T , Weight>::removeArc(uint fromNode, uint toNode)
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

template<class T, class Weight>  inline bool GWeightedGraph<T , Weight>::existsArc(uint fromNode, uint toNode) const
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
template<class T, class Weight>  inline Weight GWeightedGraph<T , Weight>::arcWeight(uint fromNode, uint toNode) const
{
	Arc* a = _nodes[fromNode]->getArc( _nodes[toNode] );
	return a->weight();
}

template<class T, class Weight> inline void GWeightedGraph<T , Weight>::setNodesMarked(bool marked)
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

template<class T, class Weight> inline void GWeightedGraph<T , Weight>::processDepthFirst(uint startNodeIndx, void(*functionPtr)(T&))
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

template<class T, class Weight> inline void GWeightedGraph<T , Weight>::processDepthFirst(Node* startNode, void(*functionPtr)(T& modifyData))
{
	functionPtr( startNode->_data );
	startNode->_marked = true;

	for(uint i=0; i< startNode->_arcs.size(); i++)
	{
		if ( startNode->_arcs[i]._pointer->_marked == false)
			processDepthFirst(  startNode->_arcs[i]._pointer, functionPtr );
	}
}

template<class T, class Weight> inline void GWeightedGraph<T , Weight>::processBreadthFirst(uint startNodeIndx, void(*functionPtr)(T&))
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


#endif	//EO  GWEIGHTEDGRAPH_H
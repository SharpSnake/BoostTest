#include <string>
#include <vector>
#include <limits>
#include <iostream>
#include <thread>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool_alloc.hpp>

#include "Watch.h"

using std::cout;
using std::endl;
using std::string;


class XTreeNode;
using XTreeNodePtr	= std::shared_ptr< XTreeNode >;
using XNode_List	= std::vector< XTreeNodePtr >;

struct Envelope2	{
	size_t West, East, South, North;
};


class XTreeNode : public std::enable_shared_from_this< XTreeNode >
{
public:
	XTreeNode( const string &name, const Envelope2 &enve )
		: m_Path( name ), m_Enve( enve )	{}

	auto& Path()	const noexcept	{ return m_Path; }
	void Add( XTreeNodePtr child )		{ m_Children.emplace_back( std::move( child ) ); }

private:
	std::uint16_t			m_State{};
	string					m_Path;
	Envelope2				m_Enve;
	XNode_List				m_Children;
};

static constexpr size_t XTREENODE_SIZE = sizeof( XTreeNode );	// 100W¸ö100M

static void test_newdel()
{
	XNode_List root;	root.reserve( 100000UL );

	for( size_t i = 0; i < 100000UL; ++i )
	{
		auto tree = std::make_shared< XTreeNode >( std::to_string( i ), Envelope2{ i, i, i, i } );
		for( size_t j = 0; j < 10; ++j )
		{
			auto child = std::make_shared< XTreeNode >( tree->Path() + std::to_string( j ), Envelope2{ j, j, j, j } );
			tree->Add( child );
		}
		root.emplace_back( std::move( tree ) );
	}

	root.clear();
}

struct pool_del
{
	pool_del( boost::object_pool< XTreeNode > &alloc )
		: m_Alloc( alloc )	{}

	void operator()( XTreeNode *ptr )	const noexcept
	{
		//m_Alloc.destroy( ptr );
		m_Alloc.free( ptr );
	}

	boost::object_pool< XTreeNode > &m_Alloc;
};

static void test_pool()
{
	boost::object_pool< XTreeNode > alloc;
	std::vector< XTreeNodePtr, boost::pool_allocator< XTreeNodePtr > > root;	root.reserve( 100000UL );

	for( size_t i = 0; i < 100000UL; ++i )
	{
		XTreeNodePtr tree( alloc.construct( std::to_string( i ), Envelope2{ i, i, i, i } ), pool_del{ alloc } );
		for( size_t j = 0; j < 10; ++j )
		{
			XTreeNodePtr child( alloc.construct( tree->Path() + std::to_string( j ), Envelope2{ j, j, j, j } ), pool_del{ alloc } );
			tree->Add( child );
		}
		root.emplace_back( std::move( tree ) );
	}

	root.clear();
}


void PoolMain()
{
	MWatch ww;
	while (true)
	{
		ww.Tic();
		test_pool();
		cout << ww.Toc() << endl;
		std::this_thread::sleep_for( std::chrono::milliseconds{ 100 } );
	}
}
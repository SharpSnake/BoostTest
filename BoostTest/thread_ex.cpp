#include <vector>
#include <utility>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#include "Macros.h"
#include "Functions.hpp"

using namespace boost;

typedef shared_mutex Mutex;                  
typedef unique_lock< Mutex >  WriteLock;
typedef shared_lock< Mutex >  ReadLock;

#define RLockThis ReadLock rLock( myLockEx )
#define WLockThis WriteLock wLock( myLockEx )

Mutex myLockEx;
std::vector< int > tex_randi( 10 );
bool tExNeedUpdate = false;

void TExRend()
{
	std::vector< int > drawlist( 10 );
	while ( 1 )
	{
		{
			RLockThis;
			if( tExNeedUpdate )
			{
				using std::swap;
				swap( drawlist, tex_randi );
				tExNeedUpdate = false;
			}
		}

		::system( "cls" );
		for( auto &i : drawlist )
			cout << i << endl;

		this_thread::sleep_for( chrono::milliseconds{ 10 } );
	}
}

void TExSchedule()
{
	std::vector< int > databuf( 10 );
	while ( 1 )
	{
		this_thread::sleep_for( chrono::milliseconds{ 500 } );

		for( auto &i : databuf )
			i = Randi( 1, 100 );

		WLockThis;
		using std::swap;
		swap( tex_randi, databuf );
		tExNeedUpdate = true;
	}
}


void ThreadExMain()
{
	thread scheduler( TExSchedule );
	thread render( TExRend );
	scheduler.detach();
	render.detach();

	while ( 1 )	// mainloop
		this_thread::sleep_for( chrono::milliseconds{ 1 } );
}
#include <stdlib.h>
#include <vector>
#include <utility>

#include <thread>
#include <chrono>
#include <mutex>
#include <shared_mutex>

#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#include "Macros.h"
#include "Functions.hpp"


//#define LIBboost

#if defined( LIBboost )
using namespace boost;
#else
using namespace std;
#endif

using Mutex = timed_mutex;
using ReadLock = unique_lock< Mutex >;
using WriteLock = unique_lock< Mutex >;

#define RLockThis ReadLock rLock( myLock, try_to_lock )
#define WLockThis WriteLock wLock( myLock )

Mutex myLock;
std::vector< int > t_randi( 10 );
bool tNeedUpdate = false;

// 多个语句加锁可能导致deadlock，一次性锁多个
// void std::lock(_Lock0& _Lk0, _Lock1& _Lk1, _LockN&... _LkN)


void TRender()
{
	std::vector< int > drawlist( 10 );
	while ( 1 )
	{
		if( tNeedUpdate )
		{
			RLockThis;
			if( rLock.owns_lock() || rLock.try_lock_for( chrono::milliseconds{ 1 } ) )
			{
				using std::swap;
				swap( drawlist, t_randi );
				tNeedUpdate = false;
			}
			else
				cout << "abandon this update" << endl;
		}
		
		::system( "cls" );
		for( auto &i : drawlist )
			cout << i << endl;

		this_thread::sleep_for( chrono::milliseconds{ 50 } );
	}
}

void TSchedule()
{
	std::vector< int > databuf( 10 );
	while ( 1 )
	{
		this_thread::sleep_for( chrono::milliseconds{ 300 } );

		for( auto &i : databuf )
			i = Randi( 1, 100 );

		WLockThis;
		using std::swap;
		swap( t_randi, databuf );
		tNeedUpdate = true;
		this_thread::sleep_for( chrono::milliseconds{ 10 } );
	}
}

void ThreadMain()
{
	thread scheduler( TSchedule );
	thread render( TRender );
	scheduler.detach();
	render.detach();

	while ( 1 )	// mainloop
		this_thread::sleep_for( chrono::milliseconds{ 1 } );
}
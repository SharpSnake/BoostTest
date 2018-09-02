#ifndef WATCH_H
#define WATCH_H

#include <Windows.h>

class MWatch
{
public:
	MWatch()
	{
		QueryPerformanceFrequency( ( LARGE_INTEGER* )&fq );
		QueryPerformanceCounter( ( LARGE_INTEGER* )&t1 );
	}

	void Reset()
	{
		QueryPerformanceCounter( ( LARGE_INTEGER* )&t1 );
	}

	void Stop()
	{
		QueryPerformanceCounter( ( LARGE_INTEGER* )&t2 );
	}

	double Millisecond()
	{
		return ( t2 - t1 ) * 1000.0 / fq;
	}

private:
	LONGLONG fq;
	LONGLONG t1;
	LONGLONG t2;
};

#endif // !WATCH_H
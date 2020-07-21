#ifndef WATCH_H
#define WATCH_H

#include <chrono>

class MWatch
{
	typedef std::chrono::high_resolution_clock	clock_type;

public:
	MWatch()	noexcept	{ Tic(); }

	void Tic()	noexcept	{ t1 = clock_type::now(); }

	template< typename Dura = std::chrono::milliseconds, typename Rep = double >
	auto Toc()	noexcept
	{
		t2 = clock_type::now();
		return std::chrono::duration_cast< std::chrono::duration< Rep, typename Dura::period > >( t2 - t1 ).count();
	}

private:
	clock_type::time_point t1;
	clock_type::time_point t2;
};

#endif // !WATCH_H
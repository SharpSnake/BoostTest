#include <Windows.h>
#include <stdexcept>
#include "Functions.hpp"

extern void SerializationMain();
extern void FormatMain();
extern void TokenizerMain();
extern void RegexMain();
extern void ThreadMain();
extern void ThreadExMain();
extern void LocaleMain();

int main( int argc, const char* argv[] )
{
	FunctionsInit();
	try
	{
		LocaleMain();
	}
	catch( const std::exception &ex )
	{
		cout << "main error:" << endl << "\t" << ex.what() << endl;
	}
	
	system( "pause" );
	return 0;
}
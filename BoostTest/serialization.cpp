#include <iostream>
#include <string>
#include <vector>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include "Macros.h"

using namespace std;

class CSPnt
{
public:
	double X, Y, Z;

	CSPnt() { cout << "default constructor" << endl; };

	CSPnt( double x, double y, double z ) 
		: X( x ), Y( y ), Z( z ) {}

	friend ostream& operator <<( ostream &o, const CSPnt &pnt )
	{
		return o << pnt.X << " " << pnt.Y << " " << pnt.Z;
	}

private:
	friend class boost::serialization::access;
	template< typename Archive >
	void serialize( Archive & ar, const unsigned int version )
	{
		ar & X;
		ar & Y;
		ar & Z;
	}
};



void SerializationMain()
{
	vector< char > buffer;
	boost::iostreams::stream< boost::iostreams::back_insert_device< vector< char > > > 
		output_stream( buffer );
	boost::archive::binary_oarchive oa( output_stream, boost::archive::no_header );
	
	CSPnt pnt1( 3.3, 5.67, -99.156 ), pnt2( 12, 7.7, 51.09 );
	oa & ( &pnt1 );
	oa & ( &pnt2 );
	output_stream.flush();

	cout << "size:\t" << buffer.size() << endl;

	boost::iostreams::basic_array_source< char > source( &buffer[ 0 ], buffer.size() );
	boost::iostreams::stream< boost::iostreams::basic_array_source < char > > 
		input_stream( source );

	CSPnt *dpnt1 = nullptr, *dpnt2 = nullptr;
	boost::archive::binary_iarchive ia( input_stream, boost::archive::no_header );
	ia & dpnt1;
	ia & dpnt2;
	cout << *dpnt1 << endl << *dpnt2 << endl;

	SafeDelete( dpnt1 );
	SafeDelete( dpnt2 );
}
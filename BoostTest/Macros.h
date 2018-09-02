#ifndef MACROS_H
#define MACROS_H


#define endl2 endl << endl
#define coute cout << endl

#define SafeDelete( p )	\
if( p )					\
{						\
	delete ( p );		\
	( p ) = nullptr;	\
}

#define SafeDeleteArray( p )\
if( p )						\
{							\
	delete[] ( p );			\
	( p ) = nullptr;			\
}

#endif // !MACROS_H
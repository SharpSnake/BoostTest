#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;


// ����ĳ����Χ�ڵ��������������b>a�ļ��
inline int Randi( const int &a, const int &b )
{
	return a + std::rand() % ( b - a + 1 /* this is the range of [ a, b ]*/ );
}

inline void FunctionsInit()
{
	std::srand( std::time( nullptr ) );
}
#endif // !FUNCTIONS_HPP
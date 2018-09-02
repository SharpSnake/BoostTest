#include <iostream>
#include <string>
#include <boost/format.hpp>

#include "Macros.h"

using namespace std;
using Format = boost::format;


struct FormatPerson
{
	string Name;
	unsigned Age;
	friend ostream& operator<<( ostream &os, const FormatPerson &p )
	{
		// ���Ǽ򵥵�ʵ�֣������԰�osǿתΪboost�ڲ���stream��֧�ָ����ӵĸ�ʽ����
		return os << p.Name << ", " << p.Age;
	}
};


void FormatMain()
{
	cout << "======= Linked multiple objects =======" << endl;
	string datetime = ( Format( "%2% - %3% - %1%" ) % 2018 % 7 % 31 ).str();
	cout << datetime << endl;

	// Ҳ֧��c���������ǰ���Ǳ�֤�������ġ�����ƥ�䡿
	cout << Format( "%05d %+03.2f %s" ) % 99 % 15.758 % "hello" << endl2;


	cout << "======= �̶��������ּ���λ =======" << endl;
	Format fNumber( "1401%05d" );
	cout << fNumber % 7 << endl;
	cout << fNumber % 963 << endl2;


	cout << "======= ƴ�ӵ�ͬʱ���ø�ʽ =======" << endl;
	// Ϊ�˷�ֹ������λ����ź͸�ʽ���ü��� $ �ָ
	cout << Format( "%1$02d��%2$02d��: %3$0.2fԪ" ) % 7 % 9 % 5.369 << endl2;


	cout << "======= Format with user-defined type =======" << endl;
	// ֻҪ�Զ�����ʵ���˱�׼���������Ϳ��ԣ�boost::format�ڲ���һ��stringstream
	FormatPerson lilei{ "����", 23 }, hanmei{ "��÷", 19 };
	std::cout << Format( "��Ա��Ϣ:\n%1%\n%2%" ) % lilei % hanmei << endl;
}


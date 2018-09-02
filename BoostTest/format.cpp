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
		// 这是简单的实现，还可以把os强转为boost内部的stream，支持更复杂的格式设置
		return os << p.Name << ", " << p.Age;
	}
};


void FormatMain()
{
	cout << "======= Linked multiple objects =======" << endl;
	string datetime = ( Format( "%2% - %3% - %1%" ) % 2018 % 7 % 31 ).str();
	cout << datetime << endl;

	// 也支持c风格的输出，前提是保证输出对象的【个数匹配】
	cout << Format( "%05d %+03.2f %s" ) % 99 % 15.758 % "hello" << endl2;


	cout << "======= 固定长度数字及补位 =======" << endl;
	Format fNumber( "1401%05d" );
	cout << fNumber % 7 << endl;
	cout << fNumber % 963 << endl2;


	cout << "======= 拼接的同时设置格式 =======" << endl;
	// 为了防止混淆，位置序号和格式设置间用 $ 分割开
	cout << Format( "%1$02d月%2$02d日: %3$0.2f元" ) % 7 % 9 % 5.369 << endl2;


	cout << "======= Format with user-defined type =======" << endl;
	// 只要自定义类实现了标准输出运算符就可以，boost::format内部有一个stringstream
	FormatPerson lilei{ "李雷", 23 }, hanmei{ "韩梅", 19 };
	std::cout << Format( "人员信息:\n%1%\n%2%" ) % lilei % hanmei << endl;
}


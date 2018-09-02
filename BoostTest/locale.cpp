#include <fstream>
#include <locale>
#include <codecvt>
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>

#include "Macros.h"
#include "Functions.hpp"
#include "Strings.hpp"

using namespace std;

wstring U8Str2WStr( const string &u8str )
{
	wstring_convert< codecvt_utf8< wchar_t > > cvt;
	return cvt.from_bytes( u8str );
}

string WStr2U8Str( const wstring &wstr )
{
	wstring_convert< codecvt_utf8< wchar_t > > cvt;
	return cvt.to_bytes( wstr );
}

string wstring2string( const wstring& str, const string &locale = "Chinese" )
{
	typedef codecvt_byname<wchar_t, char, mbstate_t> F;
	wstring_convert<F> strCnv(new F(locale));

	return strCnv.to_bytes(str);
}


void LocaleMain()
{
	//===========================================================================
	// 1、控制台代码页的概念
	//	950		繁体中文
	//	65001	UTF-8代码页
	//	936		简体中文默认的GBK（默认）
	//	932		日语
	system( "chcp" );		// 显式当前的代码页

	system( "chcp 936" );	// 指定代码页
	wstring wtest = L"中国人锤鍊";
	wcout << wtest << endl;	// wstring是unicode，直接输出到GBK会导致wcout失效
	if( !wcout )
		wcout.clear();

	// 办法就是设置转换器
	wcout.imbue( locale( "Chinese" ) );
	wcout << wtest << endl;

	system( "chcp 65001" );
	wcout << wtest << endl;	// unicode -> utf8乱码
	wcout.imbue( locale( locale( "Chinese" ), new codecvt_utf8< wchar_t >() ) );
	wcout << wtest << endl;

	// 2、窄字符串与控制台
	system( "chcp 936" );
	string stest = u8"中国人锤鍊";
	cout << stest << endl;
	cout << wstring2string( U8Str2WStr( stest ) ) << endl;


	StringsEnv AutoStrEnv;

	ifstream reader( "D:\\Kindle\\书籍制作\\Calibre繁体转简体批处理完全重制版beta3_2.csr" );
	ofstream writer( "D:\\Kindle\\书籍制作\\Calibre繁体转简体-单字.csr" );

	string line1, line2;
	while ( !reader.eof() )
	{
		getline( reader, line1 );
		getline( reader, line2 );

		wstring wline1 = U8Str2WStr( line1 );
		boost::algorithm::trim( wline1 );
		string ss = WStr2Str( wline1 );
		wstring wss = Str2WStr( ss );
		
		if( wline1.size() == 1 )
		{
			writer << line1 << endl;
			writer << line2 << endl2;
		}

		getline( reader, line1 );
	}

	writer.close();
	reader.close();
}
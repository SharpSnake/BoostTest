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
	// 1������̨����ҳ�ĸ���
	//	950		��������
	//	65001	UTF-8����ҳ
	//	936		��������Ĭ�ϵ�GBK��Ĭ�ϣ�
	//	932		����
	system( "chcp" );		// ��ʽ��ǰ�Ĵ���ҳ

	system( "chcp 936" );	// ָ������ҳ
	wstring wtest = L"�й��˴��";
	wcout << wtest << endl;	// wstring��unicode��ֱ�������GBK�ᵼ��wcoutʧЧ
	if( !wcout )
		wcout.clear();

	// �취��������ת����
	wcout.imbue( locale( "Chinese" ) );
	wcout << wtest << endl;

	system( "chcp 65001" );
	wcout << wtest << endl;	// unicode -> utf8����
	wcout.imbue( locale( locale( "Chinese" ), new codecvt_utf8< wchar_t >() ) );
	wcout << wtest << endl;

	// 2��խ�ַ��������̨
	system( "chcp 936" );
	string stest = u8"�й��˴��";
	cout << stest << endl;
	cout << wstring2string( U8Str2WStr( stest ) ) << endl;


	StringsEnv AutoStrEnv;

	ifstream reader( "D:\\Kindle\\�鼮����\\Calibre����ת������������ȫ���ư�beta3_2.csr" );
	ofstream writer( "D:\\Kindle\\�鼮����\\Calibre����ת����-����.csr" );

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
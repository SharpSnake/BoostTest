#include <iostream>
#include <string>
#include <vector>
#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string.hpp>

#include "Macros.h"

using namespace std;

template< typename Container >
void PrintStrings( const Container &strs )
{
	for( const auto &i : strs )
		cout << i << endl;
	cout << endl;
}

// ����汾�У��յķָ���������
inline void split_regex_ex( vector< string > &Result, const std::string &Input, 
	const boost::regex &Rx )
{
	using SplitIterator = boost::algorithm::split_iterator< std::string::const_iterator >;

	for( SplitIterator i = make_split_iterator( Input, regex_finder( Rx ) ); 
		i !=  SplitIterator(); ++i )
	{
		if( i->size() )
			Result.push_back( string( i->begin(), i->end() ) );
	}
}


void TokenizerMain()
{
	//===========================================================================
	// 1�������ַ�����ķָ�
	using CharSeparator = boost::char_separator< char >;
	using CharTokenizer = boost::tokenizer< CharSeparator >;


	cout << "======= ���켰������� =======" << endl;
	// Ҫ�ָ���ַ���������һ���������ٵ���ֵ���ַ�������������Ҫ�ᴩ�����ָ����
	string str1 = "Hello, Boost C++ Libraries!";

	// Ĭ�ϰ��ո�ָ�����Żᵥ���ָ����
	CharTokenizer tokDefault( str1 );

	// ��ʹ�÷ָ���ǰ��str�����б䶯�����򽫻�ȡ�����벻���Ľ��
	// Ϊ�ˣ����Խ�������浽һ���ȶ����������ץ��ʱ��ʹ�÷ָ���
	vector< string > tok2vec( tokDefault.begin(), tokDefault.end() );
	PrintStrings( tok2vec );


	cout << "======= ���÷ָ��� =======" << endl;
	string str2 = "if lilei = tom; then hanmei = lili;";

	// ����1���������ڷָ���ַ���ע���ǵ����ַ������ǰ��ַ����ָ�
	// ����2���������ַ���ָ�����ַ��������ָ�
	// ����3��Ĭ����drop_empty_tokens���������յķָ���
	CharSeparator cSep{ " =", ";", boost::drop_empty_tokens };
	CharTokenizer tok2( str2, cSep );
	PrintStrings( tok2 );


	//===========================================================================
	// 2����һ��С�ַ�����Ϊ�ָ�����Ҫ�õ�regex
	cout << "======= C# String.Split like =======" << endl;
	string str3 = "if lileiequalto tom; then hanmei ==lili;";

	// �ָ�����boost��regex��ʾ������ַ����ָ�����|�����
	boost::regex strSep( " |equalto|==|;" );

	// �ָ�������һ�������boostĬ�Ͽ��ַ����Ľ��Ҳ�᷵�أ�
	vector< string > splitresult;
	boost::algorithm::split_regex( splitresult, str3, strSep );
	PrintStrings( splitresult );

	cout << "split result without empty tokens:" << endl;
	vector< string > splitresult_without_empty;
	split_regex_ex( splitresult_without_empty, str3, strSep );
	PrintStrings( splitresult_without_empty );
}


